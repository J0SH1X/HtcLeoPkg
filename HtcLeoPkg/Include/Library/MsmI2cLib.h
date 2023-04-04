/*
 * Filename: i2c_memory_dump.c
 * Description: A program that dumps a memory region via I2C
 * Author: [Author Name]
 * Date: [Date]
 */

#ifndef I2C_MEMORY_DUMP_H
#define I2C_MEMORY_DUMP_H

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
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define MEMORY_REGION_ADDRESS 0xA9900000
#define MEMORY_REGION_LENGTH 0x00100000

void EFIAPI msm_set_i2c_mux(int mux_to_i2c);
static struct msm_i2c_pdata i2c_pdata;
EFI_STATUS dumpI2CMemory(void);
RETURN_STATUS EFIAPI MsmI2cInitialize(VOID);

#endif  // I2C_MEMORY_DUMP_H
