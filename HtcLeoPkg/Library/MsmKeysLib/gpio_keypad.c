/*
 * Copyright (c) 2009-2010, Google Inc. All rights reserved.
 * Copyright (c) 2009-2012, Code Aurora Forum. All rights reserved.
 * Copyright (c) 2011-2012, Shantanu Gupta <shans95g@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Google, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <Library/bits.h>
#include <stdlib.h>
#include <string.h>
#include <Library/keys.h>
#include <Library/gpio.h>
#include <Library/gpio_keypad.h>
#include <Library/event.h>
#include <Library/timer_kernel.h>
#include <Library/qcom_lk.h>
#include <Library/iomap.h>
#include <Library/timer_platform.h>
#include <Library/InterruptsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/list.h>
#include <Library/compiler.h>
#include <Library/thread.h>
#include <Library/timer_kernel.h>
#include <Library/timer_platform.h>
#include <Library/InterruptsLib.h>
#include <Library/lk_types.h>
#include <Library/assert.h>
#include <Library/ButtonsLib.h>
#include <Library/HtcLeoPlatformLib.h>
#include <Library/BootReason.h>

struct gpio_kp {
	struct gpio_keypad_info *keypad_info;
	struct timer timer;
	event_t full_scan;
	int current_output;
	unsigned int some_keys_pressed:2;
	unsigned long keys_pressed[0];
};

static struct gpio_kp *keypad;

static void check_output(struct gpio_kp *kp, int out, int polarity)
{
	struct gpio_keypad_info *kpinfo = kp->keypad_info;
	int key_index;
	int in;
	int gpio;
	int changed = 0;

	key_index = out * kpinfo->ninputs;
	for (in = 0; in < kpinfo->ninputs; in++, key_index++) {
		gpio = kpinfo->input_gpios[in];
		changed = 0;
		if (gpio_get(gpio) ^ !polarity) {
			if (kp->some_keys_pressed < 3)
				kp->some_keys_pressed++;
			changed = !bitmap_set(kp->keys_pressed, key_index);
		} else {
			changed = bitmap_clear(kp->keys_pressed, key_index);
		}
		if (changed) {
			int state = bitmap_test(kp->keys_pressed, key_index);
			keys_post_event(kpinfo->keymap[key_index], state);
			if (kpinfo->notify_fn)
				kpinfo->notify_fn(kpinfo->keymap[key_index], state);
		}
	}
	// sets up the right state for the next poll cycle
	gpio = kpinfo->output_gpios[out];
	if (kpinfo->flags & GPIOKPF_DRIVE_INACTIVE) {
		gpio_set(gpio, !polarity);
	} else {
		gpio_config(gpio, GPIO_INPUT);
	}
}

static enum handler_return
gpio_keypad_timer_func(struct timer *timer, time_t now, void *arg)
{
	struct gpio_kp *kp = keypad;
	struct gpio_keypad_info *kpinfo = kp->keypad_info;
	int polarity = ! !(kpinfo->flags & GPIOKPF_ACTIVE_HIGH);
	int out;
	int gpio;

	out = kp->current_output;
	if (out == kpinfo->noutputs) {
		out = 0;
		kp->some_keys_pressed = 0;
	} else {
		check_output(kp, out, polarity);
		out++;
	}

	kp->current_output = out;
	if (out < kpinfo->noutputs) {
		gpio = kpinfo->output_gpios[out];

		if (kpinfo->flags & GPIOKPF_DRIVE_INACTIVE) {
			gpio_set(gpio, polarity);
		} else {
			gpio_config(gpio, polarity ? GPIO_OUTPUT : 0);
		}
		timer_set_oneshot(timer, kpinfo->settle_time, gpio_keypad_timer_func, NULL);
		goto done;
	} /*else*/
	if (kp->some_keys_pressed || 1) {
		timer_set_oneshot(timer, kpinfo->poll_time, gpio_keypad_timer_func, NULL);
		goto done;
	}
	
done:
	return INT_RESCHEDULE;
}

void gpio_keypad_init(struct gpio_keypad_info *kpinfo)
{
	int key_count;
	int output_val;
	int output_cfg;
	int i;
	int len;

	ASSERT(kpinfo->keymap && kpinfo->input_gpios && kpinfo->output_gpios);
	key_count = kpinfo->ninputs * kpinfo->noutputs;

	len = sizeof(struct gpio_kp) + (sizeof(unsigned long) *	BITMAP_NUM_WORDS(key_count));
	//keypad = malloc(len);
	keypad = AllocatePool(len);
	ASSERT(keypad);

	memset(keypad, 0, len);
	keypad->keypad_info = kpinfo;

	output_val = (!!(kpinfo->flags & GPIOKPF_ACTIVE_HIGH))^(!!(kpinfo->flags & GPIOKPF_DRIVE_INACTIVE));
	output_cfg = kpinfo->flags & GPIOKPF_DRIVE_INACTIVE ? GPIO_OUTPUT : 0;
	for (i = 0; i < kpinfo->noutputs; i++) {
		gpio_set(kpinfo->output_gpios[i], output_val);
		gpio_config(kpinfo->output_gpios[i], output_cfg);
	}
	for (i = 0; i < kpinfo->ninputs; i++) {
		gpio_config(kpinfo->input_gpios[i], GPIO_INPUT);
	}
	keypad->current_output = kpinfo->noutputs;
	
	timer_initialize(&keypad->timer);
	htcleo_key_bkl_pwr(1);
	//htcleo_reboot(FASTBOOT_MODE);
	//timer_set_oneshot(&keypad->timer, 0, gpio_keypad_timer_func, NULL);
}
