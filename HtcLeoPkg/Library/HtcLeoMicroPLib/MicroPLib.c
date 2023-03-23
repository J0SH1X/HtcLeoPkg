//#include <Base.h>

// #include <Library/LKEnvLib.h>

// #include <Library/ClockLib.h>
// #include <Library/TargetClockLib.h>
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
#include <Chipset/timer.h>
#include <Library/gpio.h>
#include <Platform/timer.h>
#include <Library/InterruptsLib.h>
#include <Kernel/timer.h>

#include <Target/microp.h>
#include <Library/HtcLeoGpio.h>


static struct microp_platform_data microp_pdata = {
	.chip = MICROP_I2C_ADDR,
	.gpio_reset = HTCLEO_GPIO_UP_RESET_N,
};

RETURN_STATUS
EFIAPI
MicroPInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init MicroPLib \n"));
	microp_i2c_probe(&microp_pdata);


  return Status;
}