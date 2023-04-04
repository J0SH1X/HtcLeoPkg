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
#include <Library/LKEnvLib.h>


static struct microp_platform_data microp_pdata = {
	.chip = MICROP_I2C_ADDR,
	.gpio_reset = HTCLEO_GPIO_UP_RESET_N,
};

void EFIAPI htcleo_led_set_mode(uint8_t mode)
{
/* Mode
 * 0 => All off
 * 1 => Solid green
 * 2 => Solid amber
 */
	uint8_t data[4];
	
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	
	switch(mode) {
		case 0x01:
			data[1] = 0x01;
			break;
		case 0x02:
			data[1] = 0x02;
			break;
		case 0x00:
		default:
			data[1] = 0x00;
	}
	
	microp_i2c_write(MICROP_I2C_WCMD_LED_CTRL, data, 2);
}

RETURN_STATUS
EFIAPI
MicroPInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "Init MicroPLib \n"));
	microp_i2c_probe(&microp_pdata);
	//DEBUG((EFI_D_ERROR, "WRITING TO I2C IN 5 SECONDS \n"));
	//mdelay(5000);
//writel(0x1CC, 0xA9900000);
  DEBUG((EFI_D_ERROR, "ABOUT TO TURN LED GREEN \n"));
  mdelay(3000);
  htcleo_led_set_mode(1);
  DEBUG((EFI_D_ERROR, "LED NOW SHOULD BE GREEN \n"));
  mdelay(20000);


  return Status;
}