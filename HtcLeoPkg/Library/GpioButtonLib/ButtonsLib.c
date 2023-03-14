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
#include <Library/gpio_keys.h>
#include <Library/qcom_qsd8250_gpio.h>
#include <Library/ButtonsLib.h>




RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init ButtonsLib"));
  DEBUG((EFI_D_ERROR, "Enabling Keypad leds"));
  gpio_set(HTCLEO_GPIO_KP_LED, 1);
  DEBUG((EFI_D_ERROR, "Keypad leds enabled"));

//   for (int i = 0; i < 10; i++){

//   }
  //Status = gpio_keys_init(&gpio_keys_pdata);

  return Status;
}