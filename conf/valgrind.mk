#valgrind stuff
VALGRIND = valgrind
VALGRIND_SUPPS = $(CONFIG_DIR)valgrind.memcheck.supp
PROFILING_RESULTS = $(patsubst $(TEST_DIR)Test%.c,$(PROFILING_RESULTS_DIR)Test%.out,$(TESTS) )
PROFILING_RESULTS_DIR = $(BUILD_DIR)profiling_results/

profile: all $(PROFILING_RESULTS)

#generate profiling data
$(PROFILING_RESULTS_DIR)%.out: $(BUILD_DIR)%.c.o.$(TARGET_EXTENSION)
	$(MKDIR) $(dir $@)
	-$(VALGRIND) --tool=callgrind --callgrind-out-file=$@  $< > /dev/null 2>&1

#execute tests
$(TEST_RESULTS_DIR)%.txt: $(BUILD_DIR)%.c.o.$(TARGET_EXTENSION)
	$(MKDIR) $(dir $@)
	-$(VALGRIND) --suppressions=$(VALGRIND_SUPPS) --gen-suppressions=all --tool=memcheck --leak-check=full $< > $@ 2>&1
