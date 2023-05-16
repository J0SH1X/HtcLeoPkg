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

#include <Chipset/iomap.h>
#include <Chipset/clock.h>
#include <Library/reg.h>
#include <Library/adm.h>
#include <Library/part.h>
#include <Library/gpio.h>

#include <Library/LKEnvLib.h>

const int rows = 3;
const int cols = 3;
int htcleo_row_gpios[] = { 33, 32, 31 };
int htcleo_col_gpios[] = { 42, 41, 40 };
int activeLow = 0x1 & 0x1;
int keys[3][3] = {
               {1,2,3},
               {4,5,6},
               {7,8,9}
};

VOID initKeyPad() {
	DEBUG((EFI_D_ERROR, "Keypad init Started!\n"));
	mdelay(3000);
	// set all cols low
	// gpio_set(htcleo_col_gpios[0], GPIO_LOW);
	// gpio_set(htcleo_col_gpios[1], GPIO_LOW);
	// gpio_set(htcleo_col_gpios[2], GPIO_LOW);
	for (int i = 0; i < ARRAY_SIZE(htcleo_col_gpios); i++){
		gpio_set(htcleo_row_gpios[i], GPIO_LOW);
	}
	DEBUG((EFI_D_ERROR, "Keyboard Col Pins set to LOW!\n"));
	mdelay(3000);
	DEBUG((EFI_D_ERROR, "Exiting 'INIT_KeyPad' function\n"));
	mdelay(3000);
}



UINT8 getKey() {
	DEBUG((EFI_D_ERROR, "Entering 'getKey' function!\n"));
	int k = 0;

	for(int i = 0; i < ARRAY_SIZE(htcleo_col_gpios); i++){
   	     gpio_set(htcleo_col_gpios[i], GPIO_LOW);
        	for(int j = 0; j < ARRAY_SIZE(htcleo_row_gpios); j++){
				int returnValue2 = gpio_get(htcleo_row_gpios[j]);
				DEBUG((EFI_D_ERROR, "returnValue2 is: %d!\n", returnValue2));
            		if(returnValue2 == GPIO_LOW){
            			mdelay(20);    //20ms debounce time
						int returnValue = gpio_get(htcleo_row_gpios[j]);
						DEBUG((EFI_D_ERROR, "returnValue is: %s!\n", returnValue));
            			while(returnValue == GPIO_LOW);
            			k = keys[j][i];
            		}else {
						DEBUG((EFI_D_ERROR, "GPIO was high!\n"));
					}
        	 }
   	gpio_set(htcleo_col_gpios[i], GPIO_HIGH); 
      	}
      	return k;
}



RETURN_STATUS
EFIAPI
KeyPadDxeInitialize(  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable)
{
	EFI_STATUS  Status = EFI_SUCCESS;

	DEBUG((EFI_D_ERROR, "Keypad init start\n"));

	initKeyPad();

// 	for(;;) {
// 	int key = getKey();
// 	if(key != 0){
// 		DEBUG((EFI_D_ERROR, "Key Index: %d\n", key));
// 	}else {
// 		DEBUG((EFI_D_ERROR, "key is null \n"));
// 	}
// 	mdelay(1000);
// }

// for (;;){
// 	int status = gpio_get(94);
// 	DEBUG((EFI_D_ERROR, "status of gpio 94 is: %d\n", status));
// 		// int status3 = gpio_get(33);
// 	// DEBUG((EFI_D_ERROR, "status of gpio 33 is: %d\n", status3));
// 	// 		int status4 = gpio_get(42);
// 	// DEBUG((EFI_D_ERROR, "status of gpio 33 is: %d\n", status4));
// 	mdelay(1000);
// }

//check volume up button code
while true{
// for (int i = 0; i < ARRAY_SIZE(htcleo_row_gpios); i++){
// 	gpio_set(htcleo_row_gpios[i], 1);
// 	for (int j = 0; j < ARRAY_SIZE(htcleo_col_gpios); i++){
// 		int status = gpio_get(htcleo_col_gpios[i]);
// 		DEBUG((EFI_D_ERROR, "gpio status is: %d!\n", status));
// 		mdelay(500);
// 		bool isPressed = (status ? 1 : 0) ^ activeLow;

// 	}
// }

// gpio_set(33,1);
// int status = gpio_get(94);
// DEBUG((EFI_D_ERROR, "GPIO 94 is: %d!\n", status));
// if (status == 0){
// 	gpio_set(48,1);
// }else {
// 	gpio_set(48,0);
// }

KeypadDeviceImplConstructor();
KeypadDeviceImplGetKeys();

}

	DEBUG((EFI_D_ERROR, "Keypad init done!\n"));

	return Status;
}
