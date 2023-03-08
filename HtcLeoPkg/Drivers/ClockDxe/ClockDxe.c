EFI_STATUS
EFIAPI
ClockDxeInitialize(
	IN EFI_HANDLE         ImageHandle,
	IN EFI_SYSTEM_TABLE   *SystemTable
)
{
	msm_clock_init();

	return EFI_SUCCESS;
}
