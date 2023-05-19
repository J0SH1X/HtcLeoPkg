#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Timer.h>
#include <Library/DebugLib.h>
#include <Library/keypad.h>

#define HTCLEO_GPIO_KP_MKOUT0    		33
#define HTCLEO_GPIO_KP_MKOUT1    		32
#define HTCLEO_GPIO_KP_MKOUT2    		31
#define HTCLEO_GPIO_KP_MPIN0     		42
#define HTCLEO_GPIO_KP_MPIN1     		41
#define HTCLEO_GPIO_KP_MPIN2     		40

#define GPIO_INPUT	0x0000
#define GPIO_OUTPUT	0x0001

#define GPIO_LEVEL	0x0000
#define GPIO_EDGE	0x0010

#define GPIO_RISING	0x0020
#define GPIO_FALLING	0x0040

#define GPIO_HIGH	0x0020
#define GPIO_LOW	0x0040

#define GPIO_PULLUP	0x0100
#define GPIO_PULLDOWN	0x0200


static uint32_t key_rowgpio[] = {
	HTCLEO_GPIO_KP_MKOUT0,
	HTCLEO_GPIO_KP_MKOUT1,
	HTCLEO_GPIO_KP_MKOUT2,
};

static uint32_t key_colgpio[] = {
	HTCLEO_GPIO_KP_MPIN0,
	HTCLEO_GPIO_KP_MPIN1,
	HTCLEO_GPIO_KP_MPIN2,
};
#define ASZ(x) ARRAY_SIZE(x)

typedef enum {
  KEY_VOLUMEUP,
  KEY_VOLUMEDOWN,
  KEY_SOFT1,
  KEY_SEND,
  KEY_CLEAR,
  KEY_BACK,
  KEY_HOME,
} HTCLEO_BUTTON_TYPES;

#define KEYMAP_INDEX(row, col) ((row)*ARRAY_SIZE(key_colgpio) + (col))
static uint16_t htcleo_keymap[ASZ(key_colgpio) * ASZ(key_rowgpio)] = {
	[KEYMAP_INDEX(0, 0)] = KEY_VOLUMEUP,	// Volume Up
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_SOFT1,		// Windows Button
	[KEYMAP_INDEX(1, 1)] = KEY_SEND,		// Dial Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,		// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_BACK,		// Back Button
	[KEYMAP_INDEX(2, 1)] = KEY_HOME,		// Home Button
};
#undef KEYMAP_INDEX

void htcleo_handle_key_bkl(){
	//handle button backlight here
}
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

EFI_EVENT TimerEvent;
EFI_TIMER_ARCH_PROTOCOL *TimerProtocol;


void gpio_keypad_init(struct gpio_keypad_info *kpinfo)
{ 
	int output_val;
	int output_cfg;
	int i;

	output_val = (!!(kpinfo->flags & GPIOKPF_ACTIVE_HIGH))^(!!(kpinfo->flags & GPIOKPF_DRIVE_INACTIVE));
	output_cfg = kpinfo->flags & GPIOKPF_DRIVE_INACTIVE ? GPIO_OUTPUT : 0;
	for (i = 0; i < kpinfo->noutputs; i++) {
		gpio_set(kpinfo->output_gpios[i], output_val);
		gpio_config(kpinfo->output_gpios[i], output_cfg);
	}
	for (i = 0; i < kpinfo->ninputs; i++) {
		gpio_config(kpinfo->input_gpios[i], GPIO_INPUT);
	}
}

EFI_STATUS KeyPadDxeInitialize(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

    DEBUG((EFI_D_ERROR, "KEYPADDXE START\n"));


	gpio_keypad_init(&htcleo_keypad_info);

	gpio_set(HTCLEO_GPIO_KP_MKOUT1,1);
	while (true){
		int status = gpio_get(HTCLEO_GPIO_KP_MPIN1);
		 DEBUG((EFI_D_ERROR, "HTCLEO_GPIO_KP_MPIN1 is: %d\n", status));
		 mdelay(1000);
	}

    

    return EFI_SUCCESS;
}


