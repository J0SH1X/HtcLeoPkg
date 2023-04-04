/*
 * Copyright (c) 2012, Shantanu Gupta <shans95g@gmail.com>
 * Based on the open source driver from HTC, Interrupts are not supported yet
 */

#include <stdlib.h>
#include <string.h>
#include <Chipset/msm_i2c.h>
#include <Library/gpio.h>
#include <Kernel/mutex.h>
#include <Target/microp.h>
#include <Chipset/timer.h>
#include <Library/DebugLib.h>

static struct microp_platform_data *pdata = NULL;
int msm_microp_i2c_status;

int microp_i2c_read(u_int8_t addr, u_int8_t *data, int length)
{
	DEBUG((EFI_D_ERROR, "MICROP_I2C_READ \n"));
	if (!pdata)
		return -1;
		
	struct i2c_msg msgs[] = {
		{.addr = pdata->chip,	.flags = 0,			.len = 1,		.buf = &addr,},
		{.addr = pdata->chip,	.flags = I2C_M_RD,	.len = length,	.buf = data, },
	};
	
	int retry;
		DEBUG((EFI_D_ERROR, "FOR LOOP ABOUT TO HAPPEN \n"));
	for (retry = 0; retry <= MSM_I2C_READ_RETRY_TIMES; retry++) {
				DEBUG((EFI_D_ERROR, "FOR LOOP ENTERED \n"));
		if (msm_i2c_xfer(msgs, 2) == 2){
			break;
			}
			DEBUG((EFI_D_ERROR, "MSM_I2C_XFER FINISHED YAY !!!!!! \n"));
			mdelay(5000);
		mdelay(5);
	}
	if (retry > MSM_I2C_WRITE_RETRY_TIMES)
		return -1;
	
	return 0;
}

#define MICROP_I2C_WRITE_BLOCK_SIZE 21
int microp_i2c_write(u_int8_t addr, u_int8_t *cmd, int length)
{
	DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE \n"));
	if (!pdata){
		DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: No pdata \n"));
		return -1;
	}
	if (length >= MICROP_I2C_WRITE_BLOCK_SIZE){
		DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: length >= 21 \n"));
		return -1;
		}
	
	u_int8_t cmd_buffer[MICROP_I2C_WRITE_BLOCK_SIZE];
	
	struct i2c_msg msg[] = {
		{.addr = pdata->chip,	.flags = 0,		.len = length + 1,	.buf = cmd_buffer,}
	};
	
	cmd_buffer[0] = addr;
	//memcpy((void *)&cmd_buffer[1], (void *)cmd, length);
	DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: CopyMem now \n"));
	CopyMem((VOID *)&cmd_buffer[1], (VOID *)cmd, (UINTN)length);
	mdelay(1000);
	
	int retry;
	for (retry = 0; retry <= MSM_I2C_WRITE_RETRY_TIMES; retry++) {
		if (msm_i2c_xfer(msg, 1) == 1){
			DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: xfer success ??? \n"));
			mdelay(2000);
			break;
			}
		mdelay(5);
	}
	if (retry > MSM_I2C_WRITE_RETRY_TIMES){
		DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: xfer timeout \n"));
		mdelay(2000);
		return -1;
	}
	DEBUG((EFI_D_ERROR, "MICROP_I2C_WRITE: returning 0 \n"));
	mdelay(1000);
	return 0;
}

int microp_read_adc(u_int8_t channel, u_int16_t *value)
{
	u_int8_t cmd[2], data[2];

	cmd[0] = 0;
	cmd[1] = 1;

	if (microp_i2c_write(MICROP_I2C_WCMD_READ_ADC_VALUE_REQ, cmd, 2) < 0) {
		//dprintf(SPEW, "%s: request adc fail!\n", __func__);
		return -1;
	}

	if (microp_i2c_read(MICROP_I2C_RCMD_ADC_VALUE, data, 2) < 0) {
		//dprintf(SPEW, "%s: read adc fail!\n", __func__);
		return -1;
	}
	
	*value = data[0] << 8 | data[1];

	return 0;
}

int microp_read_gpi_status(u_int16_t *status)
{
	u_int8_t data[2];

	if (microp_i2c_read(MICROP_I2C_RCMD_GPIO_STATUS, data, 2) < 0) {
		//dprintf(SPEW, "%s: read fail!\n", __func__);
         //DEBUG((EFI_D_ERROR, "%s: read fail!\n", __func__));
		return -1;
	}
	
	*status = (data[0] << 8) | data[1];
	
	return 0;
}

int microp_interrupt_get_status(u_int16_t *interrupt_mask)
{
	u_int8_t data[2];
	int ret = -1;

	ret = microp_i2c_read(MICROP_I2C_RCMD_GPI_INT_STATUS, data, 2);
	if (ret < 0) {
        //DEBUG((EFI_D_ERROR, "%s: read interrupt status fail\n",  __func__));
		return ret;
	}

	*interrupt_mask = data[0]<<8 | data[1];

	return 0;
}

int microp_interrupt_enable( u_int16_t interrupt_mask)
{
	u_int8_t data[2];
	int ret = -1;

	data[0] = interrupt_mask >> 8;
	data[1] = interrupt_mask & 0xFF;
	ret = microp_i2c_write(MICROP_I2C_WCMD_GPI_INT_CTL_EN, data, 2);

	if (ret < 0){
		//dprintf(INFO, "%s: enable 0x%x interrupt failed\n", __func__, interrupt_mask);
       // DEBUG((EFI_D_ERROR, "%s: disable 0x%x interrupt failed\n", __func__, gpo_mask));
        }
	return ret;
}

int microp_interrupt_disable(u_int16_t interrupt_mask)
{
	u_int8_t data[2];
	int ret = -1;

	data[0] = interrupt_mask >> 8;
	data[1] = interrupt_mask & 0xFF;
	ret = microp_i2c_write(MICROP_I2C_WCMD_GPI_INT_CTL_DIS, data, 2);

	if (ret < 0){
		//dprintf(INFO, "%s: disable 0x%x interrupt failed\n", __func__, interrupt_mask);
        //DEBUG((EFI_D_ERROR, "%s: disable 0x%x interrupt failed\n", __func__, gpo_mask));
        }
	return ret;
}

int microp_read_gpo_status(u_int16_t *status)
{
	u_int8_t data[2];

	if (microp_i2c_read(MICROP_I2C_RCMD_GPIO_STATUS, data, 2) < 0) 
	{
		//dprintf(CRITICAL, "%s: read failed!\n", __func__);
		return -1;
	}

	*status = (data[0] << 8) | data[1];

	return 0;
}

int microp_gpo_enable(u_int16_t gpo_mask)
{
	u_int8_t data[2];
	int ret = -1;

	data[0] = gpo_mask >> 8;
	data[1] = gpo_mask & 0xFF;
	ret = microp_i2c_write(MICROP_I2C_WCMD_GPO_LED_STATUS_EN, data, 2);

	if (ret < 0){
		//dprintf(CRITICAL, "%s: enable 0x%x interrupt failed\n", __func__, gpo_mask);
       // DEBUG((EFI_D_ERROR, "%s: disable 0x%x interrupt failed\n", __func__, gpo_mask));
    }
	return ret;
}

int microp_gpo_disable(u_int16_t gpo_mask)
{
	u_int8_t data[2];
	int ret = -1;

	data[0] = gpo_mask >> 8;
	data[1] = gpo_mask & 0xFF;
	ret = microp_i2c_write(MICROP_I2C_WCMD_GPO_LED_STATUS_DIS, data, 2);

	if (ret < 0){
		//dprintf(CRITICAL, "%s: disable 0x%x interrupt failed\n", __func__, gpo_mask);
        //DEBUG((EFI_D_ERROR, "%s: disable 0x%x interrupt failed\n", __func__, gpo_mask));
}
	return ret;
}

static int als_power_control=0;
//struct mutex capella_cm3602_lock;
// int capella_cm3602_power(int pwr_device, u_int8_t enable)
// {
// 	unsigned int old_status = 0;
// 	u_int16_t interrupts = 0;
// 	int ret = 0, on = 0;
	
// 	mutex_acquire(&capella_cm3602_lock);
// 	if(pwr_device==PS_PWR_ON) { // Switch the Proximity IRQ
// 		if(enable) {
// 			microp_gpo_enable(PS_PWR_ON);
// 			ret = microp_interrupt_get_status(&interrupts);
// 			if (ret < 0) {
// 				printf("read interrupt status fail\n");
// 				return ret;
// 			}
// 			interrupts |= IRQ_PROXIMITY;
// 			ret = microp_interrupt_enable(interrupts);
// 		}
// 		else {
// 			interrupts |= IRQ_PROXIMITY;
// 			ret = microp_interrupt_disable(interrupts);
// 			microp_gpo_disable(PS_PWR_ON);
// 		}
// 		if (ret < 0) {
// 			printf("failed to enable gpi irqs\n");
// 			return ret;
// 		}
// 	}
	
// 	old_status = als_power_control;
// 	if (enable)
// 		als_power_control |= pwr_device;
// 	else
// 		als_power_control &= ~pwr_device;

// 	on = als_power_control ? 1 : 0;
// 	if (old_status == 0 && on)
// 		microp_gpo_enable(LS_PWR_ON);
// 	else if (!on)
// 		microp_gpo_disable(LS_PWR_ON);
		
// 	mutex_release(&capella_cm3602_lock);
	
// 	return ret;
// }

static int microp_function_initialize(void)
{    
	u_int16_t stat, interrupts = 0;
	int ret;

	ret = microp_interrupt_enable(interrupts);
	if (ret < 0) {
		//dprintf(CRITICAL, "%s: failed to enable gpi irqs\n", __func__);
		goto err_irq_en;
	}

	microp_read_gpi_status(&stat);
	return 0;

err_irq_en:
	return ret;
}

void microp_i2c_probe(struct microp_platform_data *kpdata)
{
	//if(!kpdata || pdata) return;

	pdata = kpdata;
	
	u_int8_t data[6];
	if (microp_i2c_read(MICROP_I2C_RCMD_VERSION, data, 2) < 0) {
		msm_microp_i2c_status = 0;
		//printf("microp get version failed!\n");
        DEBUG((EFI_D_ERROR, "microp get version failed! \n"));
		return;
	}
	//printf("HTC MicroP 0x%02X\n", data[0]);
    DEBUG((EFI_D_ERROR, "HTC MicroP 0x%02X\n", data[0]));
	msm_microp_i2c_status = 1;
}