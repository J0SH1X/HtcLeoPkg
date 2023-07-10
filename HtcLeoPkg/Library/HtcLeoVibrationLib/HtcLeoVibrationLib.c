/** @file
*
*  Copyright (c) 2023, J0SH1X <aljoshua.hell@gmail.com>. orginal code by cedesmith and all lk authors. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/DebugLib.h>
#include <Library/qcom_qsd8250_iomap.h>
#include <Library/HtcLeoGpio.h>
#include <Library/qcom_lk.h>
#include <Library/qcom_qsd8250_timer.h>
#include <Library/gpio.h>


VOID EFIAPI htcleo_vibrate(unsigned enable){
  gpio_set(HTCLEO_GPIO_VIBRATOR_ON, enable);
}

VOID EFIAPI htcleo_vibrate_once(){
  htcleo_vibrate(1);
  mdelay(100);
  htcleo_vibrate(0);
}

VOID EFIAPI htcleo_vibrate_timer(unsigned msecs){
  htcleo_vibrate(1);
  mdelay(msecs);
  htcleo_vibrate(0);
}