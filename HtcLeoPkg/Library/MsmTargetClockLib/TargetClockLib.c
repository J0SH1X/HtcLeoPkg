#include <Library/TargetClockLib.h>
#include <Library/qcom_qsd8250_clock.h>

RETURN_STATUS
EFIAPI
MsmTargetClockLibInitialize(VOID)
{
  EFI_STATUS         Status = EFI_SUCCESS;

  msm_clock_init();

  return Status;
}