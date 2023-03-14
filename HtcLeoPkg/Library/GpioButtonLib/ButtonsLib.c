//#include <Base.h>

// #include <Library/LKEnvLib.h>

// #include <Library/ClockLib.h>
// #include <Library/TargetClockLib.h>
#include <Target/gpio_keys.h>
#include <Library/gpio.h>
#include <Library/ButtonsLib.h>


static struct gpio_keys_pdata htcleo_gpio_key_pdata = {
	.nr_keys = 1,
	.poll_time = 20,
	.debounce_time = 40,
	.gpio_key = &gpio_keys_tbl[0],
};

static struct gpio_key gpio_keys_tbl[] = {
	{
		.polarity 		= 1,
		.key_code 		= KEY_POWER,
		.gpio_nr 		= HTCLEO_GPIO_POWER_KEY,
		.notify_fn 		= &htcleo_handle_key_bkl,
	},
};

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