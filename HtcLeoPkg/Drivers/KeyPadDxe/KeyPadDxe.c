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

#define HTCLEO_POWER_KP_GPIO			94

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

//volume down 32,41 31,41
//home 33,41, 32,41
//dial 33,41 31,41



#define KEYMAP_INDEX(row, col) ((row)*3 + (col))
static HTCLEO_BUTTON_TYPE htcleo_keymap[3 * 3] = {
	[KEYMAP_INDEX(0, 0)] = KEY_VOLUMEUP,	//  Volume up
	[KEYMAP_INDEX(0, 1)] = KEY_VOLUMEDOWN,	// Volume Down
	[KEYMAP_INDEX(1, 0)] = KEY_SOFT1,		// Windows Button, mapped
	[KEYMAP_INDEX(1, 1)] = KEY_SEND,		// Dial Button
	[KEYMAP_INDEX(1, 2)] = KEY_CLEAR,		// Hangup Button
	[KEYMAP_INDEX(2, 0)] = KEY_BACK,		// Back Button
	[KEYMAP_INDEX(2, 1)] = KEY_HOME,		// Home Button, mapped
};

void htcleo_handle_key_bkl(){
	//handle button backlight here
}
static struct gpio_keypad_info htcleo_keypad_info = {
	.keymap        	= htcleo_keymap,
	.output_gpios  	= key_rowgpio,
	.input_gpios   	= key_colgpio,
	.noutputs      	= 3,
	.ninputs      	= 3,
	.settle_time   	= 40,
	.poll_time     	= 20,
	.flags        	= GPIOKPF_DRIVE_INACTIVE,
	.notify_fn     	= &htcleo_handle_key_bkl,
};
#undef ASZ

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

	while (TRUE){
		//handle power key first

				int powerKeyStatus = gpio_get(HTCLEO_POWER_KP_GPIO);
					if (powerKeyStatus == 0){
						//power key is a seperate GPIO 
						DEBUG((EFI_D_ERROR, "power key pressed\n"));
					}

		for (int i = 0; i < 3; i++){
			gpio_set(key_rowgpio[i], 0);
				for (int j = 0; j < 3; j++){
					int status = gpio_get(key_colgpio[j]);

					if (status == 0){
						HTCLEO_BUTTON_TYPE button = htcleo_keymap[KEYMAP_INDEX(i,j)];
	switch (button) {
    case KEY_SOFT1:
        // Windows key
        DEBUG((EFI_D_ERROR, "windows key pressed\n"));
        break;
    case KEY_SEND:
        // Dial key
        DEBUG((EFI_D_ERROR, "dial key pressed\n"));
        break;
    case KEY_BACK:
        // Back key
        DEBUG((EFI_D_ERROR, "back key pressed\n"));
        break;
    case KEY_HOME:
        // Home key
        DEBUG((EFI_D_ERROR, "home key pressed\n"));
        break;
    case KEY_VOLUMEDOWN:
        // Volume down
        DEBUG((EFI_D_ERROR, "volume down key pressed\n"));
        break;
    case KEY_VOLUMEUP:
        // Volume up
        DEBUG((EFI_D_ERROR, "volume up key pressed\n"));
        break;
    default:
        // Handle any other cases
        break;
}
					}

					
				}
			gpio_set(key_rowgpio[i], 1);
			mdelay(100);
			//DEBUG((EFI_D_ERROR, "\n"));

		}
	}
    

    return EFI_SUCCESS;
}


