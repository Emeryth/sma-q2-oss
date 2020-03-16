TEST_RESULTS_DIR = $(BUILD_DIR)test_results/
TEST_OUTPUT = $(BUILD_DIR)test/
TEST_DIR = test/
TEST_RUNNERS = $(TEST_DIR)test_runners/
RUBY = ruby

include conf/valgrind.mk
include conf/cppcheck.mk

#unity testing files
TESTS = $(wildcard $(TEST_DIR)*.c)
TEST_RESULTS = $(patsubst $(TEST_DIR)Test%.c,$(TEST_RESULTS_DIR)Test%.txt,$(TESTS) )
TEST_OBJS = $(TESTS:%=$(BUILD_DIR)%.o)

test: all $(TEST_OBJS) $(TEST_RESULTS) $(CPPCHECK_RESULTS)
	@echo ""
	@echo "-----------------------ANALYSIS AND TESTING SUMMARY-----------------------"
	@echo `find $(TEST_RESULTS_DIR) -type f -exec grep IGNORE {} \;|wc -l` "tests ignored"
	@echo "`find $(TEST_RESULTS_DIR) -type f -exec grep IGNORE {} \;`"
	@echo `find $(TEST_RESULTS_DIR) -type f -exec grep FAIL {} \;|wc -l` "tests failed"
	@echo "`find $(TEST_RESULTS_DIR) -type f -exec grep FAIL {} \;`"
	@echo `find $(TEST_RESULTS_DIR) -type f -exec grep PASS {} \;|wc -l` "tests passed"
	@echo ""
	@echo "`grep -Poh 'ERROR SUMMARY:\K ([0-9]+)' $(TEST_RESULTS_DIR)*| awk '{ SUM += $$1} END { print SUM }'` memory leak(s) detected"
	@echo ""
	@echo `find $(CPPCHECK_RESULTS_DIR) -type f -exec grep warning {} \;|wc -l` "code warnings"
	@echo `find $(CPPCHECK_RESULTS_DIR) -type f -exec grep warning {} \;`
	@echo `find $(CPPCHECK_RESULTS_DIR) -type f -exec grep error {} \;|wc -l` "code errors"
	@echo "`find $(CPPCHECK_RESULTS_DIR) -type f -exec grep error {} \;`"


#unity test runners
$(TEST_RUNNERS)%.c:: $(TEST_DIR)%.c
	$(MKDIR) $(dir $@)
	$(RUBY) $(UNITY_ROOT)/auto/generate_test_runner.rb $< $@

#build the test runners
$(BUILD_DIR)%.c.o.$(TARGET_EXTENSION): $(TEST_OUTPUT)%.c.o
	$(CC) -g -o $@ $^ $(INC_DIRS) $(DIRECTIVES) $(OBJECTS) $(TEST_RUNNERS)$(basename $(notdir $<))