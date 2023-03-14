//#include <Base.h>

// #include <Library/LKEnvLib.h>

// #include <Library/ClockLib.h>
// #include <Library/TargetClockLib.h>
#include <Library/gpio_keys.h>
#include <Library/gpio.h>
#include <Library/ButtonsLib.h>




RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init ButtonsLib"));
  DEBUG((EFI_D_ERROR, "Enabling Keypad leds"));
  gpio_set(HTCLEO_GPIO_KP_LED, !!enable);
  DEBUG((EFI_D_ERROR, "Keypad leds enabled"));

//   for (int i = 0; i < 10; i++){

//   }
  //Status = gpio_keys_init(&gpio_keys_pdata);
  ASSERT_EFI_ERROR(Status);

  return Status;
}