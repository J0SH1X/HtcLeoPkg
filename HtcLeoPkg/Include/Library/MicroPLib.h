#ifndef _MICROP_H_
#define _MICROP_H_

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
#include <Library/gpio.h>
#include <Library/InterruptsLib.h>

#include <Target/microp.h>
#include <Library/HtcLeoGpio.h>

extern int msm_microp_i2c_status;
extern struct microp_platform_data microp_pdata;

RETURN_STATUS EFIAPI MicroPInitialize(VOID);

#endif /* _MICROP_H_ */
