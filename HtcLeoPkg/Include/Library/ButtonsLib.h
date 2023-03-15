#ifndef __LIBRARY_QCOM_BUTTONS_LIB_H__
#define __LIBRARY_QCOM_BUTTONS_LIB_H__

#include <Library/HtcLeoGpio.h>
#include <Library/keys.h>
#include <Library/gpio_keypad.h>
#define TARGETLIBLOGNAME "ButtonsLib: "

RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID);

//extern QCOM_CLOCK_PROTOCOL *gClock;
 VOID EFIAPI htcleo_key_bkl_pwr(unsigned enable);
 VOID EFIAPI htcleo_vibrator(unsigned enable);

#endif