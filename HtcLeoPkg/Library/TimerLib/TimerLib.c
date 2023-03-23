/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>

extern UINT32 TimerBase(INTN Timer);

#define GPT_REG(off) (0xAC100000 + (off))//MSM_GPT_BASE

#define GPT_MATCH_VAL        GPT_REG(0x0000)
#define GPT_COUNT_VAL        GPT_REG(0x0004)
#define GPT_ENABLE           GPT_REG(0x0008)
#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define GPT_CLEAR            GPT_REG(0x000C)

#define DGT_MATCH_VAL        GPT_REG(0x0010)
#define DGT_COUNT_VAL        GPT_REG(0x0014)
#define DGT_ENABLE           GPT_REG(0x0018)
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1
#define DGT_CLEAR            GPT_REG(0x001C)

#define SPSS_TIMER_STATUS    GPT_REG(0x0034)

void mdelay(unsigned msecs)
{
	msecs *= 33;

  MmioWrite32(GPT_CLEAR, 0);
  MmioWrite32(GPT_ENABLE, 0);
  while(MmioRead32(GPT_COUNT_VAL) != 0) ;

  MmioWrite32(GPT_ENABLE, GPT_ENABLE_EN);
  while(MmioRead32(GPT_COUNT_VAL) < msecs) ;

  MmioWrite32(GPT_ENABLE, 0);
  MmioWrite32(GPT_CLEAR, 0);
}

void udelay(unsigned usecs)
{
	usecs = (usecs * 33 + 1000 - 33) / 1000;

  MmioWrite32(GPT_CLEAR, 0);
  MmioWrite32(GPT_ENABLE, 0);
  while(MmioRead32(GPT_COUNT_VAL) != 0) ;

  MmioWrite32(GPT_ENABLE, GPT_ENABLE_EN);
  while(MmioRead32(GPT_COUNT_VAL) < usecs) ;

  MmioWrite32(GPT_ENABLE, 0);
  MmioWrite32(GPT_CLEAR, 0);
}

RETURN_STATUS
EFIAPI
TimerConstructor (
  VOID
  )
{
//  UINT32 TimerBaseAddress = TimerBase(0);
  DEBUG ((EFI_D_ERROR, "Timer init not done!!!\n"));
  return EFI_SUCCESS;
}

UINTN
EFIAPI
MicroSecondDelay (
  IN  UINTN MicroSeconds
  )
{
  mdelay(MicroSeconds/1000);
  return MicroSeconds;
}

UINTN
EFIAPI
NanoSecondDelay (
  IN  UINTN NanoSeconds
  )
{
  mdelay(NanoSeconds/1000);
  return NanoSeconds;
}

UINT64
EFIAPI
GetTimeInNanoSecond (
  IN      UINT64  Ticks
  )
{
  UINT64  NanoSeconds = 0;
  return NanoSeconds;
}

UINT64
EFIAPI
GetPerformanceCounter (
  VOID
  )
{ 
  // Todo
  return (UINT64)-1;
}

UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT UINT64  *StartValue,  OPTIONAL
  OUT UINT64  *EndValue     OPTIONAL
  )
{
  // Todo
  if (StartValue != NULL) {
    // Timer starts with the reload value
    *StartValue = (UINT64)10000;
  }
  
  if (EndValue != NULL) {
    // Timer counts up to 0xFFFFFFFF
    *EndValue = 0xFFFFFFFF;
  }
  
  return PcdGet64(PcdEmbeddedPerformanceCounterFrequencyInHz);
}
