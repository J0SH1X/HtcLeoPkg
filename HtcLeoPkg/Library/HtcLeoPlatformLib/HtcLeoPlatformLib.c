/** @file
*
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/DebugLib.h>
#include <Library/pcom.h>
#include <Library/qcom_lk.h>
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

#define LK_BOOTREASON_ADDR 		0x2FFB0000
#define MARK_LK_TAG 	0X004B4C63



VOID EFIAPI reboot (unsigned rebootReason){
  msm_proc_comm(PCOM_RESET_CHIP, &rebootReason, 0);
  for(;;) ;
}

VOID EFIAPI htcleo_reboot(unsigned rebootReason){
  writel(rebootReason, LK_BOOTREASON_ADDR);
	writel(rebootReason^MARK_LK_TAG, LK_BOOTREASON_ADDR + 4);
  reboot(rebootReason);
}

VOID EFIAPI htcleo_shutdown (){
  msm_proc_comm(PCOM_POWER_DOWN, 0, 0);
	for (;;) ;
}