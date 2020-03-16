
patch:
	( \
		$(CD) $(NRF5_SDK_DIR)/components/drivers_nrf \
		$(PATCH) -s -p0 < patch/twi.patch \
	)