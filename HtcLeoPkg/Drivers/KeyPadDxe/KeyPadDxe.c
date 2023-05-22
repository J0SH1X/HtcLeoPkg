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




#define KEYMAP_INDEX(row, col) ((row)*3 + (col))
static HTCLEO_BUTTON_TYPE htcleo_keymap[3 * 3] = {
	[KEYMAP_INDEX(0, 0)] = KEY_BACK,		//  Back Button again, mapped
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_BACK,		// Back Button, mapped
	[KEYMAP_INDEX(1, 1)] = KEY_HOME,		// Home Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,		// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_SOFT1,		// Windows Button, mapped
	[KEYMAP_INDEX(2, 1)] = KEY_SEND,		// Dial Button , mapped
};

// HTCLEO_BUTTON_TYPE leoKeys[9] = {
// 	KEY_BACK,
// 	K
// }

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
		DEBUG((EFI_D_ERROR, "Setting GPIO %d to %d\n",kpinfo->output_gpios[i], output_val ));
		gpio_set(kpinfo->output_gpios[i], output_val);
		DEBUG((EFI_D_ERROR, "Config GPIO %d to %d\n",kpinfo->output_gpios[i], output_cfg ));
		gpio_config(kpinfo->output_gpios[i], output_cfg);
	}
	for (i = 0; i < kpinfo->ninputs; i++) {
		gpio_config(kpinfo->input_gpios[i], GPIO_INPUT);
	}
}

EFI_STATUS KeyPadDxeInitialize(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

    DEBUG((EFI_D_ERROR, "KEYPADDXE START\n"));


	//gpio_keypad_init(&htcleo_keypad_info);
	while (TRUE){
		for (int i = 0; i < 3; i++){
			//DEBUG((EFI_D_ERROR, "setting GPIO %d to %d\n", key_rowgpio[i], 1));
			gpio_set(key_rowgpio[i], 1);
				for (int j = 0; j < 3; j++){
					int status = gpio_get(key_colgpio[j]);

					if (status == 0){
						HTCLEO_BUTTON_TYPE button = htcleo_keymap[KEYMAP_INDEX(i,j)];
						//DEBUG((EFI_D_ERROR, "button pressed is %s\n", button));

						if (button == KEY_HOME){
							DEBUG((EFI_D_ERROR, "button pressed, i: %d, j: %d\n", i, j));
						}
					}else {
					//DEBUG((EFI_D_ERROR, "GPIO %d is %d\n", key_colgpio[j], status));
					}
				}
			gpio_set(key_rowgpio[i], 0);

		}
		mdelay(3000);
	}
    

    return EFI_SUCCESS;
}


