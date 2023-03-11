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

#include <Library/qcom_lk.h>

#include <Library/qcom_qsd8250_iomap.h>
#include <Library/qcom_qsd8250_irqs.h>
#include <Library/qcom_qsd8250_clock.h>
#include <Library/qcom_qsd8250_timer.h>
#include <ArmGenericTimerCounterLib.h>

#ifdef MDE_CPU_ARM
#define MultU64xN MultU64x32
#else
#define MultU64xN MultU64x64
#endif

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1


//doc
//https://github.com/torvalds/linux/blob/master/Documentation/devicetree/bindings/arm/msm/timer.txt
//https://github.com/tsgan/qualcomm/blob/master/timer.c
//https://github.com/groeck/coreboot/blob/a234f45601e6e85a5179ec9cc446f070b86f425b/src/soc/qualcomm/ipq806x/timer.c


//inline void delay_ticks(unsigned ticks)
void delay_ticks(unsigned ticks)
{
	ticks += readl(GPT_COUNT_VAL);
	while(readl(GPT_COUNT_VAL) < ticks)
		;
}

void mdelay(unsigned msecs)
{
	delay_ticks((msecs * 33));
}

void udelay(unsigned usecs)
{
	delay_ticks(((usecs * 33 + 1000 - 33) / 1000));
}


RETURN_STATUS
EFIAPI
TimerConstructor (
  VOID
  )
{
	//DEBUG ((EFI_D_ERROR, "TimerLib:TimerConstructor\n"));
	return EFI_SUCCESS;
}

//����
UINTN
EFIAPI
MicroSecondDelay (
  IN  UINTN MicroSeconds
  )
{
	mdelay(MicroSeconds);
	return MicroSeconds;
}


/*VOID Set_DGT_Enable(int en)
{
	UINT32 DATA = MmioRead32(DGT_ENABLE);
	UINT32 DATA_NEW;
	if(en)
	{
		DATA_NEW = DATA | (1 << 0);
	}
	else
	{
		DATA_NEW = DATA & ~(1 << 0);
	}
	MmioWrite32 (DGT_ENABLE, DATA_NEW);
}

VOID Set_DGT_ClrOnMatch(int en)
{
	UINT32 DATA = MmioRead32(DGT_ENABLE);
	UINT32 DATA_NEW;
	if(en)
	{
		DATA_NEW = DATA | (1 << 1);
	}
	else
	{
		DATA_NEW = DATA & ~(1 << 1);
	}
	MmioWrite32 (DGT_ENABLE, DATA_NEW);
}*/


UINTN
EFIAPI
NanoSecondDelay (
  IN  UINTN NanoSeconds
  )
{
	udelay(NanoSeconds);
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

/**
  Retrieves the 64-bit frequency in Hz and the range of performance counter
  values.
  If StartValue is not NULL, then the value that the performance counter starts
  with immediately after is it rolls over is returned in StartValue. If
  EndValue is not NULL, then the value that the performance counter end with
  immediately before it rolls over is returned in EndValue. The 64-bit
  frequency of the performance counter in Hz is always returned. If StartValue
  is less than EndValue, then the performance counter counts up. If StartValue
  is greater than EndValue, then the performance counter counts down. For
  example, a 64-bit free running counter that counts up would have a StartValue
  of 0 and an EndValue of 0xFFFFFFFFFFFFFFFF. A 24-bit free running counter
  that counts down would have a StartValue of 0xFFFFFF and an EndValue of 0.
  @param  StartValue  The value the performance counter starts with when it
                      rolls over.
  @param  EndValue    The value that the performance counter ends with before
                      it rolls over.
  @return The frequency in Hz.
**/
UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT      UINT64                    *StartValue,  OPTIONAL
  OUT      UINT64                    *EndValue     OPTIONAL
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


/**
  Converts elapsed ticks of performance counter to time in nanoseconds.
  This function converts the elapsed ticks of running performance counter to
  time value in unit of nanoseconds.
  @param  Ticks     The number of elapsed ticks of running performance counter.
  @return The elapsed time in nanoseconds.
**/
UINT64
EFIAPI
GetTimeInNanoSecond (
  IN      UINT64                     Ticks
  )
{
  UINT64  NanoSeconds = 0;
  /*UINT32  Remainder;
  UINT32  TimerFreq;

  TimerFreq = PcdGet32(PcdArmArchTimerFreqInHz);
  //
  //          Ticks
  // Time = --------- x 1,000,000,000
  //        Frequency
  //
  NanoSeconds = MultU64xN (
                  DivU64x32Remainder (
                    Ticks,
                    TimerFreq,
                    &Remainder),
                  1000000000U
                  );

  //
  // Frequency < 0x100000000, so Remainder < 0x100000000, then (Remainder * 1,000,000,000)
  // will not overflow 64-bit.
  //
  NanoSeconds += DivU64x32 (
                   MultU64xN (
                     (UINT64) Remainder,
                     1000000000U),
                   TimerFreq
                   );*/

  return NanoSeconds;
}