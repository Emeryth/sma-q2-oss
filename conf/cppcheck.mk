#cppcheck
CPPCHECK = /usr/bin/cppcheck
CPPCHECK_FILES := $(shell find $(SRC_DIR) -maxdepth 2 \( -iname "*.c" ! -iname "*.pb.c" \))
CPPCHECK_FLAGS = -q --enable=all --inconclusive --suppress=missingIncludeSystem --suppress='*:$(LIB_DIR)/*' --inline-suppr
CPPCHECK_RESULTS_DIR = $(BUILD_DIR)cppcheck_results/
CPPCHECK_RESULTS = $(CPPCHECK_FILES:%=$(CPPCHECK_RESULTS_DIR)%.txt)

#execute cppcheck
$(CPPCHECK_RESULTS_DIR)%.c.txt: %.c
	@$(MKDIR) $(dir $@)
	$(CPPCHECK) $(INC_FLAGS) $(DIRECTIVES)  $(CPPCHECK_FLAGS) $< > $@ 2>&1
	@echo -n "."