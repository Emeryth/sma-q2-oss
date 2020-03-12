ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif
#todo: on osx you need sed like this: sed -i ""

OUTPUT_FILENAME = $(notdir $(shell pwd))

#directories
BUILD_DIR = .build/
RELEASE_DIR = release/
PROFILING_RESULTS_DIR = $(BUILD_DIR)profiling_results/
CPPCHECK_RESULTS_DIR = $(BUILD_DIR)cppcheck_results/
TEST_RESULTS_DIR = $(BUILD_DIR)test_results/
TEST_OUTPUT = $(BUILD_DIR)test/
CONFIG_DIR = conf/
SRC_DIRS = src
LIB_DIR = lib
NRF5_SDK_DIR = $(LIB_DIR)/nRF5_SDK_11.0.0_89a8197
TEMPLATE_PATH = $(NRF5_SDK_DIR)/components/toolchain/gcc
TEST_DIRS = test/
TEST_RUNNERS = $(TEST_DIRS)test_runners/
INCLUDE_DEST = $(RELEASE_DIR)include/
SRC_DEST = $(RELEASE_DIR)src/

#non auto-included dirs
LIB_DIRS += $(LIB_DIR)
LIB_DIRS += $(LIB_DIR)/unity/src
LIB_DIRS += $(LIB_DIR)/freertos/include
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/config
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/timer
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/scheduler
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/fifo
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/fstorage/config
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/delay
LIB_DIRS += $(NRF5_SDK_DIR)/components/softdevice/s132/headers/nrf52
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/util
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/uart
LIB_DIRS += $(NRF5_SDK_DIR)/components/ble/common
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/pstorage
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/uart
LIB_DIRS += $(NRF5_SDK_DIR)/components/device
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/button
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/pwm
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/fstorage
LIB_DIRS += $(NRF5_SDK_DIR)/components/libraries/experimental_section_vars
LIB_DIRS += $(NRF5_SDK_DIR)/components/softdevice/s132/headers
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/gpiote
LIB_DIRS += $(NRF5_SDK_DIR)/components/ble/ble_services/ble_nus
LIB_DIRS += $(NRF5_SDK_DIR)/components/toolchain/CMSIS/Include
LIB_DIRS += $(NRF5_SDK_DIR)/components/toolchain/gcc
LIB_DIRS += $(NRF5_SDK_DIR)/components/toolchain
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/common
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/clock
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/spi_master
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/wdt
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/saadc
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/ppi
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/timer
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/twi_master
LIB_DIRS += $(NRF5_SDK_DIR)/components/drivers_nrf/hal
LIB_DIRS += $(NRF5_SDK_DIR)/components/ble/ble_advertising
LIB_DIRS += $(NRF5_SDK_DIR)/components/softdevice/common/softdevice_handler

#protobuf files
SRCPB := $(shell find $(SRC_DIRS) -maxdepth 2 \( -iname "*.proto" \))
PBMODELS = $(patsubst $(SRC_DIRS)%.proto,$(SRC_DIRS)%.pb.c,$(SRCPB) )
PB_OBJS = $(patsubst $(SRC_DIRS)%.proto,$(BUILD_DIR)$(SRC_DIRS)%.pb.c.o,$(SRCPB) )

#unity testing files
SRCT = $(wildcard $(TEST_DIRS)*.c)
RUNNERS = $(patsubst $(TEST_DIRS)%.c,$(TEST_RUNNERS)%.c,$(SRCT) )
TEST_RESULTS = $(patsubst $(TEST_DIRS)Test%.c,$(TEST_RESULTS_DIR)Test%.txt,$(SRCT) )
PROFILING_RESULTS = $(patsubst $(TEST_DIRS)Test%.c,$(PROFILING_RESULTS_DIR)Test%.out,$(SRCT) )
TEST_OBJS = $(SRCT:%=$(BUILD_DIR)%.o)
UNITY_ROOT=/lib/unity

#valgrind stuff
VALGRIND = /usr/bin/valgrind
VALGRIND_SUPPS = $(CONFIG_DIR)valgrind.memcheck.supp

#project source files
SRCS := $(shell find $(LIB_DIRS) -maxdepth 2 \( -iname "*.c" ! -iname "*.pb.c" \))
SRCS += $(shell find $(SRC_DIRS) \( -iname "*.c" ! -iname "*.pb.c" \))

#some files in nrf libraries are mutually exclusive to filter out unneeded files libs
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/hal/nrf_ecb.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/pstorage/pstorage_nosd.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/pstorage/pstorage_raw.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/spi_master/spi_5W_master.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/twi_master/deprecated/twi_sw_master.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/twi_master/deprecated/twi_hw_master.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/scheduler/app_scheduler_serconn.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer_rtx.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer_ble_gzll.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/uart/app_uart.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/toolchain/system_nrf51.c,$(SRCS))
SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/toolchain/system_nrf51422.c,$(SRCS))
SRCS := $(filter-out $(wildcard lib/nRF5_SDK_11.0.0_89a8197/external/freertos/*),$(SRCS))

C_OBJECTS = $(SRCS:%=$(BUILD_DIR)%.o) $(PB_OBJS)
INC_DIRS := $(shell find $(LIB_DIRS) $(SRC_DIRS) -maxdepth 2 -type d)

#cppcheck
CPPCHECK = cppcheck
CPPCHECK_FILES := $(shell find $(SRC_DIRS) -maxdepth 2 \( -iname "*.c" ! -iname "*.pb.c" \))
CPPCHECK_FLAGS = -q --enable=all --inconclusive --suppress=missingIncludeSystem
CPPCHECK_RESULTS = $(CPPCHECK_FILES:%=$(CPPCHECK_RESULTS_DIR)%.txt)

#misc variables
DIRECTIVES = -DPB_FIELD_16BIT -DLOG_USE_COLOR -DUNITY_OUTPUT_COLOR -DDEBUG_LEVEL=1
FLAGS = -fPIC
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -I./src
CURRENT_DIR = $(notdir $(shell pwd))
CP = cp
CFLAGS = $(INC_FLAGS) $(FLAGS) $(DIRECTIVES) --std=gnu99
LIBS = lib/libarm_cortexM4lf_math.a

ASM_SOURCE_FILES  = $(abspath $(NRF5_SDK_DIR)/components/toolchain/gcc/gcc_startup_nrf52.s)
ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(NRF5_SDK_DIR)/components/toolchain/gcc
ASM_OBJECTS = $(addprefix $(BUILD_DIR), $(ASM_SOURCE_FILE_NAMES:.s=.o) )
vpath %.s $(ASM_PATHS)
OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

#flags common to all targets
CFLAGS += -DNRF52
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DBOARD_CUSTOM
CFLAGS += -DNRF52_PAN_12 -DNRF52_PAN_15 -DNRF52_PAN_58 -DNRF52_PAN_55 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_30 -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_53
CFLAGS += -DNRF52_PAN_20 -DNRF52_PAN_64 -DNRF52_PAN_62 -DNRF52_PAN_63
CFLAGS += -DS132
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSWI_DISABLE0
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Og -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
CFLAGS += -L./pah8series
CFLAGS += -DARM_MATH_CM4

# keep every function in separate section. This will allow linker to dump unused functions
LINKER_SCRIPT=$(LIB_DIR)/ble_app_uart_gcc_nrf52.ld
LDFLAGS += -Xlinker -Map=$(BUILD_DIR)/application.map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys  -u _printf_float

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DBOARD_CUSTOM
ASMFLAGS += -DNRF52_PAN_12 -DNRF52_PAN_15 -DNRF52_PAN_58 -DNRF52_PAN_55 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_30 -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_53
ASMFLAGS += -DNRF52_PAN_20 -DNRF52_PAN_64 -DNRF52_PAN_62 -DNRF52_PAN_63
#ASMFLAGS += -DNRF_LOG_USES_UART=1
ASMFLAGS += -DS132
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -D__HEAP_SIZE=512
ASMFLAGS += -DARM_MATH_CM4

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar

.PHONY: all
.PHONY: test
.PHONY: profile
.PHONY: jupyter
.PHONY: pythondeps
.PHONY: clean
.PHONY: cppcheck
.PHONY: reset
.PHONY: flash

all: $(PBMODELS) $(RUNNERS) $(OBJECTS) cppcheck

reset:
	./bin/fw-tool-pi-nrfjprog.sh/nrfjprog.sh --clockspeed 10000 --family nRF52 --reset
flash: all $(RELEASE_DIR)$(OUTPUT_FILENAME).hex
	./bin/fw-tool-pi-nrfjprog.sh/nrfjprog.sh --clockspeed 10000 --family nRF52 --flash  $(RELEASE_DIR)$(OUTPUT_FILENAME).hex
	./bin/fw-tool-pi-nrfjprog.sh/nrfjprog.sh --clockspeed 10000 --family nRF52 --reset

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

profile: all $(PROFILING_RESULTS)

$(RELEASE_DIR)$(OUTPUT_FILENAME).bin: $(BUILD_DIR)$(CURRENT_DIR).out
	arm-none-eabi-objcopy -O binary $< $@

$(RELEASE_DIR)$(OUTPUT_FILENAME).hex: $(BUILD_DIR)$(CURRENT_DIR).out
	$(MKDIR) $(dir $@)
	arm-none-eabi-objcopy -O ihex $< $@

#link objects into an so to be included elsewhere
$(BUILD_DIR)$(CURRENT_DIR).out: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $@

#generate profiling data
$(PROFILING_RESULTS_DIR)%.out: $(BUILD_DIR)%.c.o.$(TARGET_EXTENSION)
	$(MKDIR) $(dir $@)
	-$(VALGRIND) --tool=callgrind --callgrind-out-file=$@  $< > /dev/null 2>&1

#execute tests
$(TEST_RESULTS_DIR)%.txt: $(BUILD_DIR)%.c.o.$(TARGET_EXTENSION)
	$(MKDIR) $(dir $@)
	-$(VALGRIND) --suppressions=$(VALGRIND_SUPPS) --gen-suppressions=all --tool=memcheck --leak-check=full $< > $@ 2>&1

#build the test runners
$(BUILD_DIR)%.c.o.$(TARGET_EXTENSION): $(TEST_OUTPUT)%.c.o
	$(CC) -g -o $@ $^ $(CFLAGS) $(OBJECTS) $(UNITY_ROOT)/src/unity.c $(TEST_RUNNERS)$(basename $(notdir $<))

# assembly
$(BUILD_DIR)%.o: %.s
	$(MKDIR) $(dir $@)
	$(CC) $(ASMFLAGS) -c $< -o $@

#execute cppcheck
$(CPPCHECK_RESULTS_DIR)%.c.txt: %.c
	$(MKDIR) $(dir $@)
	$(CPPCHECK) $(INC_FLAGS) $(DIRECTIVES) $(CPPCHECK_FLAGS) $< > $@ 2>&1

# c source
$(BUILD_DIR)%.c.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# protocol buffer models
src/protobuff/%.pb.c:: $(SRCPB) Pipfile.lock
	/usr/bin/protoc --plugin=protoc-gen-nanopb=./lib/nanopb/generator/protoc-gen-nanopb --nanopb_out=. $<
	/usr/bin/protoc --python_out=. $<
	find src/protobuff -name "*.pb.c" -exec sed -i 's|src/protobuff/||' {} \;
	mv src/protobuff/*_pb2.py notebooks

jupyter:
	( \
		jupyter-lab notebooks/ --allow-root --no-mathjax --ip=0.0.0.0 --port=8888 --no-browser --NotebookApp.token='' \
	)

#unity test runners
$(TEST_RUNNERS)%.c:: $(TEST_DIRS)%.c
	$(MKDIR) $(dir $@)
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb $< $@

clean:
	$(CLEANUP) src/protobuff/*.pb.*
	$(CLEANUP) notebooks/*_pb2.py
	$(CLEANUP)r $(BUILD_DIR)
	$(CLEANUP)r $(RELEASE_DIR)

.PRECIOUS: $(TEST_RESULTS_DIR)%.txt
.PRECIOUS: $(PROFILING_RESULTS_DIR)%.txt
.PRECIOUS: $(BUILD_DIR)%.c.o.out
.PRECIOUS: $(PBMODELS)
