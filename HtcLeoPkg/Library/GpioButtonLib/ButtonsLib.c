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
#include <Library/gpio_keys.h>
#include <Library/qcom_qsd8250_gpio.h>
#include <Library/gpio.h>
#include <Library/ButtonsLib.h>
#include <Library/timer.h>
#include <Library/InterruptsLib.h>
#include <Library/keys.h>
#include <Library/timer_kernel.h>
#include <Library/HtcLeoPlatformLib.h>
#include <Library/BootReason.h>




static enum handler_return keys_bkl_off(struct timer *timer, time_t now, void *arg)
{
	//timer_cancel(timer);
  //todo impliment timer lib this sucks
	//htcleo_key_bkl_pwr(0);
  //todo impliment this freaking function properly
	
	return INT_RESCHEDULE;
}

struct timer keyp_bkl;
static unsigned keys_pressed[3] = {0, 0, 0};

static void htcleo_handle_key_bkl(unsigned key_code, unsigned state)
{
	if (key_code == KEY_VOLUMEUP || key_code == KEY_VOLUMEDOWN)
		return;
	
	if (state) {
		if ((keys_pressed[0] == 0) && (keys_pressed[1] == 0) && (keys_pressed[2] == 0))
			htcleo_key_bkl_pwr(1);

			 if (!(keys_pressed[0]))	keys_pressed[0] = key_code;
		else if (!(keys_pressed[1]))	keys_pressed[1] = key_code;
		else if (!(keys_pressed[2])) 	keys_pressed[2] = key_code;
	
		//timer_cancel(&keyp_bkl); toDo need to impliment timer lib from lk
	} else {
			 if (key_code == keys_pressed[0]) 	keys_pressed[0] = 0;
		else if (key_code == keys_pressed[1])	keys_pressed[1] = 0;
		else if (key_code == keys_pressed[2])	keys_pressed[2] = 0;

		if ((keys_pressed[0] == 0) && (keys_pressed[1] == 0) && (keys_pressed[2] == 0))
			timer_set_oneshot(&keyp_bkl, 800, keys_bkl_off, NULL);	
	}

}

static u_int32_t key_rowgpio[] = {
	HTCLEO_GPIO_KP_MKOUT0,
	HTCLEO_GPIO_KP_MKOUT1,
	HTCLEO_GPIO_KP_MKOUT2,
};

static u_int32_t key_colgpio[] = {
	HTCLEO_GPIO_KP_MPIN0,
	HTCLEO_GPIO_KP_MPIN1,
	HTCLEO_GPIO_KP_MPIN2,
};

#define ASZ(x) ARRAY_SIZE(x)
#define KEYMAP_INDEX(row, col) ((row)*ARRAY_SIZE(key_colgpio) + (col))
static u_int16_t htcleo_keymap[ASZ(key_colgpio) * ASZ(key_rowgpio)] = {
	[KEYMAP_INDEX(0, 0)] = KEY_VOLUMEUP,	// Volume Up
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_SOFT1,		// Windows Button
	[KEYMAP_INDEX(1, 1)] = KEY_SEND,		// Dial Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,		// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_BACK,		// Back Button
	[KEYMAP_INDEX(2, 1)] = KEY_HOME,		// Home Button
};
#undef KEYMAP_INDEX
static struct gpio_keypad_info htcleo_keypad_info = {
	.keymap        	= htcleo_keymap,
	.output_gpios  	= key_rowgpio,
	.input_gpios   	= key_colgpio,
	.noutputs      	= ASZ(key_rowgpio),
	.ninputs      	= ASZ(key_colgpio),
	.settle_time   	= 40,
	.poll_time     	= 20,
	.flags        	= GPIOKPF_DRIVE_INACTIVE,
	.notify_fn     	= &htcleo_handle_key_bkl,
};
#undef ASZ

static struct gpio_key gpio_keys_tbl[] = {
	{
		.polarity 		= 1,
		.key_code 		= KEY_POWER,
		.gpio_nr 		= HTCLEO_GPIO_POWER_KEY,
		.notify_fn 		= &htcleo_handle_key_bkl,
	},
};

static struct gpio_keys_pdata htcleo_gpio_key_pdata = {
	.nr_keys = 1,
	.poll_time = 20,
	.debounce_time = 40,
	.gpio_key = &gpio_keys_tbl[0],
};

VOID EFIAPI
htcleo_key_bkl_pwr(unsigned enable)
{
  gpio_set(HTCLEO_GPIO_KP_LED, enable);
}

VOID EFIAPI 
htcleo_vibrator(unsigned enable)
{
  gpio_set(HTCLEO_GPIO_VIBRATOR_ON, enable);
}

RETURN_STATUS
EFIAPI
MsmGpioButtonInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init ButtonsLib"));
  DEBUG((EFI_D_ERROR, "Init MsmKeysLib"));
  keys_init();
  DEBUG((EFI_D_ERROR, "Keys_init done"));
  DEBUG((EFI_D_ERROR, "Init Timer"));
  timer_initialize(&keyp_bkl);
  DEBUG((EFI_D_ERROR, "Timer init done"));
  DEBUG((EFI_D_ERROR, "Init Gpio Keys"));
  gpio_keys_init(&htcleo_gpio_key_pdata);
  DEBUG((EFI_D_ERROR, "Gpio Keys init done"));
  DEBUG((EFI_D_ERROR, "Init Gpio Keypad"));
  //gpio_keypad_init(&htcleo_keypad_info);
  htcleo_reboot(FASTBOOT_MODE);
  DEBUG((EFI_D_ERROR, "Gpio Keypad init done"));


  return Status;
}