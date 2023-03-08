EFI_STATUS
EFIAPI
SdccLibInitialize(
	VOID
);

int sdcc_read_data(mmc_t *mmc, mmc_cmd_t *cmd, mmc_data_t *data);