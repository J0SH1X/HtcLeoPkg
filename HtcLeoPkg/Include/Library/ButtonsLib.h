#ifndef __LIBRARY_QCOM_BUTTONS_LIB_H__
#define __LIBRARY_QCOM_BUTTONS_LIB_H__

#define HTCLEO_GPIO_KP_LED 48
#define TARGETLIBLOGNAME "ButtonsLib: "

// static struct gpio_keys_pdata htcleo_gpio_key_pdata = {
// 	.nr_keys = 1,
// 	.poll_time = 20,
// 	.debounce_time = 40,
// 	.gpio_key = &gpio_keys_tbl[0],
// };

// static struct gpio_key gpio_keys_tbl[] = {
// 	{
// 		.polarity 		= 1,
// 		.key_code 		= KEY_POWER,
// 		.gpio_nr 		= HTCLEO_GPIO_POWER_KEY,
// 		.notify_fn 		= &htcleo_handle_key_bkl,
// 	},
// };

//#include <Protocol/QcomClock.h>

RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID);

//extern QCOM_CLOCK_PROTOCOL *gClock;

#endif