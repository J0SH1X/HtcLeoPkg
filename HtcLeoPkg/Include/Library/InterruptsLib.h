/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef __LIBRARY_INTERRUPTS_H
#define __LIBRARY_INTERRUPTS_H

typedef UINT32 uint32_t;

enum handler_return {
  INT_NO_RESCHEDULE = 0,
  INT_RESCHEDULE,
};

struct arm_iframe {
	uint32_t spsr;
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;
	uint32_t pc;
};

typedef enum handler_return (*int_handler)(VOID *arg);

INTN mask_interrupt(UINTN vector);
INTN unmask_interrupt(UINTN vector);
VOID register_int_handler(UINTN vector, int_handler handler, VOID *arg);
//VOID register_int_handler(UINTN vector, int handler, VOID *arg);

VOID platform_init_interrupts(VOID);
VOID htcleo_disable_interrupts(VOID);
//enum handler_return platform_irq(struct arm_iframe *frame);

#endif
