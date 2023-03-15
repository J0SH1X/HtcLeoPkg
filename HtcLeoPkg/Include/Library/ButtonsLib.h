#ifndef __LIBRARY_QCOM_BUTTONS_LIB_H__
#define __LIBRARY_QCOM_BUTTONS_LIB_H__

#include <Library/HtcLeoGpio.h>
#define TARGETLIBLOGNAME "ButtonsLib: "


#define ASZ(x) ARRAY_SIZE(x)
#define KEYMAP_INDEX(row, col) ((row)*ARRAY_SIZE(key_colgpio) + (col))
static uint16_t htcleo_keymap[ASZ(key_colgpio) * ASZ(key_rowgpio)] = {
	[KEYMAP_INDEX(0, 0)] = KEY_VOLUMEUP,	// Volume Up
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_SOFT1,		// Windows Button
	[KEYMAP_INDEX(1, 1)] = KEY_SEND,		// Dial Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,		// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_BACK,		// Back Button
	[KEYMAP_INDEX(2, 1)] = KEY_HOME,		// Home Button
};
#undef KEYMAP_INDEX
static struct gpio_keypad_info htcleo_keypad_info = {
	.keymap        	= htcleo_keymap,
	.output_gpios  	= key_rowgpio,
	.input_gpios   	= key_colgpio,
	.noutputs      	= ASZ(key_rowgpio),
	.ninputs      	= ASZ(key_colgpio),
	.settle_time   	= 40,
	.poll_time     	= 20,
	.flags        	= GPIOKPF_DRIVE_INACTIVE,
	.notify_fn     	= &htcleo_handle_key_bkl,
};
#undef ASZ

static struct gpio_key gpio_keys_tbl[] = {
	{
		.polarity 		= 1,
		.key_code 		= KEY_POWER,
		.gpio_nr 		= HTCLEO_GPIO_POWER_KEY,
		.notify_fn 		= &htcleo_handle_key_bkl,
	},
};

static struct gpio_keys_pdata htcleo_gpio_key_pdata = {
	.nr_keys = 1,
	.poll_time = 20,
	.debounce_time = 40,
	.gpio_key = &gpio_keys_tbl[0],
};

RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID);

//extern QCOM_CLOCK_PROTOCOL *gClock;

#endif