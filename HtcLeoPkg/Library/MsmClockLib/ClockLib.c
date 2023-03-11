//#include <Base.h>

#include <Library/LKEnvLib.h>

#include <Library/ClockLib.h>
#include <Library/TargetClockLib.h>

/*#include "clock_p.h"

QCOM_CLOCK_PROTOCOL *gClock = NULL;

STATIC QCOM_CLOCK_PROTOCOL mInternalClock = {
    clk_get,        clk_enable,         clk_disable,
    clk_get_rate,   clk_set_rate,       clk_set_parent,
    clk_get_parent, clk_get_set_enable, clk_reset,
};*/

RETURN_STATUS
EFIAPI
MsmClockLibInitialize(VOID)
{
  EFI_STATUS         Status;
  /*struct clk_lookup *clist = NULL;
  unsigned           num   = 0;

  gClock = &mInternalClock;*/

  Status = MsmTargetClockLibInitialize();
  ASSERT_EFI_ERROR(Status);

  return Status;
}