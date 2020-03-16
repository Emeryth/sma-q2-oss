NRFJPROG = bin/fw-tool-pi-nrfjprog.sh/nrfjprog.sh
ADAFRUIT_NRF_UTIL = adafruit-nrfutil

reset:
	 $(NRFJPROG) --clockspeed 10000 --family nRF52 --reset
flash: all $(RELEASE_DIR)$(PROJECT_NAME).hex
	$(NRFJPROG) --clockspeed 10000 --family nRF52 --flash  $(RELEASE_DIR)$(PROJECT_NAME).hex
	$(NRFJPROG) --clockspeed 10000 --family nRF52 --reset

dfu: all Pipfile.lock $(RELEASE_DIR)$(PROJECT_NAME).hex
	$(PIPENV) run $(ADAFRUIT_NRF_UTIL) dfu genpkg --application $(RELEASE_DIR)$(PROJECT_NAME).hex --application-version 0xffffffff --dev-revision 0xffff --dev-type 0xffff --sd-req 136 $(RELEASE_DIR)app_dfu_package.zip