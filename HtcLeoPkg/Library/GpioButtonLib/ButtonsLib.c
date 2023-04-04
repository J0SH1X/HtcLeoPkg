//#include <Base.h>

// #include <Library/LKEnvLib.h>

// #include <Library/ClockLib.h>
// #include <Library/TargetClockLib.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Target/gpio_keys.h>
#include <Chipset/gpio.h>
#include <Library/gpio.h>




RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init ButtonsLib"));
  DEBUG((EFI_D_ERROR, "Enabling Keypad leds"));
  gpio_set(HTCLEO_GPIO_KP_LED, 1);
  DEBUG((EFI_D_ERROR, "Keypad leds enabled"));

  Status = gpio_keys_init(&gpio_keys_pdata);

  return Status;
}