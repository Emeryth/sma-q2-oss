PROJECT_NAME = $(notdir $(shell pwd))

.PHONY: all
.PHONY: test
.PHONY: profile
.PHONY: jupyter
.PHONY: clean
.PHONY: cppcheck
.PHONY: reset
.PHONY: flash

include conf/common.mk
include conf/compile_setup.mk
include conf/compile.mk
include conf/tests.mk
include conf/programming.mk
include conf/jupyter.mk

clean:
	$(CLEANUP) src/protobuff/*.pb.*
	$(CLEANUP) notebooks/*_pb2.py
	$(CLEANUP)r $(BUILD_DIR)
	$(CLEANUP)r $(RELEASE_DIR)

.PRECIOUS: $(TEST_RESULTS_DIR)%.txt
.PRECIOUS: $(PROFILING_RESULTS_DIR)%.txt
.PRECIOUS: $(BUILD_DIR)%.c.o.out
.PRECIOUS: $(PBMODELS)
