#ifndef __LIBRARY_QCOM_BUTTONS_LIB_H__
#define __LIBRARY_QCOM_BUTTONS_LIB_H__

#define HTCLEO_GPIO_KP_LED 48
#define TARGETLIBLOGNAME "ButtonsLib: "

//#include <Protocol/QcomClock.h>

RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID);

//extern QCOM_CLOCK_PROTOCOL *gClock;

#endif