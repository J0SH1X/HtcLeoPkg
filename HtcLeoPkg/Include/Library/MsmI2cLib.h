#ifndef _MSM_I2C_H_
#define _MSM_I2C_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Chipset/gpio.h>
#include <Kernel/timer.h>
#include <Library/InterruptsLib.h>
#include <Kernel/timer.h>
#include <Chipset/msm_i2c.h>
#include <Chipset/clock.h>
#include <Chipset/irqs.h>
#include <Chipset/iomap.h>
#include <Library/HtcLeoGpio.h>
#include <Library/pcom.h>

void EFIAPI msm_set_i2c_mux(int mux_to_i2c);
RETURN_STATUS EFIAPI MsmI2cInitialize(VOID);

#endif //_MY_HEADER_H_
