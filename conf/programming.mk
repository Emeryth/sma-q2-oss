NRFJPROG = bin/fw-tool-pi-nrfjprog.sh/nrfjprog.sh

reset:
	 $(NRFJPROG) --clockspeed 10000 --family nRF52 --reset
flash: all $(RELEASE_DIR)$(PROJECT_NAME).hex
	$(NRFJPROG) --clockspeed 10000 --family nRF52 --flash  $(RELEASE_DIR)$(PROJECT_NAME).hex
	$(NRFJPROG) --clockspeed 10000 --family nRF52 --reset

