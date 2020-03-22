WGET = wget
WGET_OPTS = -O
UNZIP = unzip
UNZIP_OPTS = -qo
NORDIC_SDK_FILE = nRF5_SDK_11.0.0_89a8197.zip
NORDIC_SDK_URL = https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/$(NORDIC_SDK_FILE)

dependencies: $(LIB_DIR)/nRF5_SDK_11.0.0_89a8197.zip submodules Pipfile.lock
	
# Git submodules handling
submodules_targets:=$(shell git config -f .gitmodules --get-regexp submodule'.*\.'path |  cut -d ' ' -f 2)
$(submodules_targets):
	git submodule update --init -- $@ && cd $@ && git checkout -f HEAD && git clean -dfx
submodules: $(submodules_targets)

$(LIB_DIR)/nRF5_SDK_11.0.0_89a8197.zip:
	$(WGET) $(WGET_OPTS) $(NRF5_SDK_DIR)$(NORDIC_SDK_FILE) $(NORDIC_SDK_URL)
	touch $@
	$(UNZIP) $(UNZIP_OPTS) -d $(NRF5_SDK_DIR) $(NRF5_SDK_DIR)$(NORDIC_SDK_FILE)