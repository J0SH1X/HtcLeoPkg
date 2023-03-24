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
#include <Kernel/timer.h>
#include <Library/InterruptsLib.h>
#include <Kernel/timer.h>
#include <Chipset/msm_i2c.h>
#include <Chipset/clock.h>
#include <Chipset/irqs.h>
#include <Chipset/iomap.h>
#include <Library/HtcLeoGpio.h>
#include <Library/pcom.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define MEMORY_REGION_ADDRESS 0xA9900000
#define MEMORY_REGION_LENGTH 0x00100000


void EFIAPI msm_set_i2c_mux(int mux_to_i2c) {
	if (mux_to_i2c) {
		pcom_gpio_tlmm_config(MSM_GPIO_CFG(GPIO_I2C_CLK, 0, MSM_GPIO_CFG_OUTPUT, MSM_GPIO_CFG_NO_PULL, MSM_GPIO_CFG_8MA), 0);
		pcom_gpio_tlmm_config(MSM_GPIO_CFG(GPIO_I2C_DAT, 0, MSM_GPIO_CFG_OUTPUT, MSM_GPIO_CFG_NO_PULL, MSM_GPIO_CFG_8MA), 0);
	} else {
		pcom_gpio_tlmm_config(MSM_GPIO_CFG(GPIO_I2C_CLK, 1, MSM_GPIO_CFG_INPUT, MSM_GPIO_CFG_NO_PULL, MSM_GPIO_CFG_2MA), 0);
		pcom_gpio_tlmm_config(MSM_GPIO_CFG(GPIO_I2C_DAT, 1, MSM_GPIO_CFG_INPUT, MSM_GPIO_CFG_NO_PULL, MSM_GPIO_CFG_2MA), 0);
	}
}

static struct msm_i2c_pdata i2c_pdata = {
	.i2c_clock = 400000,
	.clk_nr	= I2C_CLK,
	.irq_nr = INT_PWB_I2C,
	.scl_gpio = GPIO_I2C_CLK,
	.sda_gpio = GPIO_I2C_DAT,
	.set_mux_to_i2c = &msm_set_i2c_mux,
	.i2c_base = (void*)MSM_I2C_BASE,
};

EFI_STATUS dumpI2CMemory(void)
{
  EFI_STATUS status;
  UINTN bufferSize = 0x00100000;

  // Allocate a buffer to hold the memory dump
  UINT8 *buffer = NULL;
  status = gBS->AllocatePool(EfiBootServicesData, bufferSize, (VOID **)&buffer);
  if (EFI_ERROR(status)) {
    DEBUG((EFI_D_ERROR, "Failed to allocate memory: %r\n", status));
    return status;
  }

  // Copy the memory region to the buffer
  gBS->CopyMem(buffer, (VOID*)(UINTN)0xA9900000, bufferSize);

  // Dump the buffer to the console
  CHAR16* bufferString = NULL;
  UnicodeSPrint(&bufferString, 0, L"Memory Dump:\n");
  DEBUG((EFI_D_INFO, bufferString));
  FreePool(bufferString);

  for (UINTN i = 0; i < bufferSize; i++) {
    if (i % 16 == 0) {
      UnicodeSPrint(&bufferString, 0, L"%08X: ", (UINT32)(0xA9900000 + i));
      DEBUG((EFI_D_INFO, bufferString));
      FreePool(bufferString);
    }
    UnicodeSPrint(&bufferString, 0, L"%02X ", buffer[i]);
    DEBUG((EFI_D_INFO, bufferString));
    FreePool(bufferString);

    if (i % 16 == 15) {
      DEBUG((EFI_D_INFO, L"\n"));
    }
  }

  FreePool(buffer);
  return EFI_SUCCESS;
}

RETURN_STATUS
EFIAPI
MsmI2cInitialize(VOID)
{

  
  EFI_STATUS         Status = EFI_SUCCESS;

//my code goes here
	msm_i2c_probe(&i2c_pdata);

  return Status;
}