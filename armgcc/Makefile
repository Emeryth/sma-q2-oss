PROJECT_NAME := sma-q2-oss

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

#NRF5_SDK_PATH ?= $(HOME)/Software/nRF5_SDK_11.0.0_89a8197
#COMPONENTS     = $(NRF5_SDK_PATH)/components
COMPONENTS     = ../components
TEMPLATE_PATH = $(COMPONENTS)/toolchain/gcc

ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

VERBOSE = 1

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
$(abspath $(COMPONENTS)/libraries/button/app_button.c) \
$(abspath $(COMPONENTS)/libraries/util/app_error.c) \
$(abspath $(COMPONENTS)/libraries/fifo/app_fifo.c) \
$(abspath $(COMPONENTS)/libraries/util/app_util_platform.c) \
$(abspath $(COMPONENTS)/libraries/timer/app_timer_freertos.c) \
$(abspath $(COMPONENTS)/libraries/fstorage/fstorage.c) \
$(abspath $(COMPONENTS)/libraries/util/nrf_assert.c) \
$(abspath $(COMPONENTS)/libraries/uart/retarget.c) \
$(abspath $(COMPONENTS)/libraries/uart/app_uart_fifo.c) \
$(abspath $(COMPONENTS)/libraries/pwm/app_pwm.c) \
$(abspath $(COMPONENTS)/drivers_nrf/saadc/nrf_drv_saadc.c) \
$(abspath $(COMPONENTS)/drivers_nrf/hal/nrf_saadc.c) \
$(abspath $(COMPONENTS)/drivers_nrf/delay/nrf_delay.c) \
$(abspath $(COMPONENTS)/drivers_nrf/common/nrf_drv_common.c) \
$(abspath $(COMPONENTS)/drivers_nrf/gpiote/nrf_drv_gpiote.c) \
$(abspath $(COMPONENTS)/drivers_nrf/ppi/nrf_drv_ppi.c) \
$(abspath $(COMPONENTS)/drivers_nrf/spi_master/nrf_drv_spi.c) \
$(abspath $(COMPONENTS)/drivers_nrf/timer/nrf_drv_timer.c) \
$(abspath $(COMPONENTS)/drivers_nrf/twi_master/nrf_drv_twi.c) \
$(abspath $(COMPONENTS)/drivers_nrf/uart/nrf_drv_uart.c) \
$(abspath $(COMPONENTS)/drivers_nrf/wdt/nrf_drv_wdt.c) \
$(abspath $(COMPONENTS)/drivers_nrf/pstorage/pstorage.c) \
$(abspath $(COMPONENTS)/drivers_nrf/clock/nrf_drv_clock.c) \
$(abspath ../main.c) \
$(abspath $(COMPONENTS)/ble/common/ble_advdata.c) \
$(abspath $(COMPONENTS)/ble/ble_advertising/ble_advertising.c) \
$(abspath $(COMPONENTS)/ble/common/ble_conn_params.c) \
$(abspath $(COMPONENTS)/ble/ble_services/ble_nus/ble_nus.c) \
$(abspath $(COMPONENTS)/ble/common/ble_srv_common.c) \
$(abspath $(COMPONENTS)/toolchain/system_nrf52.c) \
$(abspath $(COMPONENTS)/softdevice/common/softdevice_handler/softdevice_handler.c) \
$(abspath ../freertos/croutine.c) \
$(abspath ../freertos/event_groups.c) \
$(abspath ../freertos/heap_2.c) \
$(abspath ../freertos/list.c) \
$(abspath ../freertos/port.c) \
$(abspath ../freertos/port_cmsis.c) \
$(abspath ../freertos/port_cmsis_systick.c) \
$(abspath ../freertos/queue.c) \
$(abspath ../freertos/tasks.c) \
$(abspath ../freertos/timers.c) \
$(abspath ../hardware/accel.c) \
$(abspath ../hardware/buttons.c) \
$(abspath ../hardware/lcd.c) \
$(abspath ../hardware/battery.c) \
$(abspath ../hardware/backlight.c) \
$(abspath ../hardware/vibration.c) \
$(abspath ../hardware/pah8002.c) \
$(abspath ../hardware/hrm.c) \
$(abspath ../gfx/nrf_gfx.c) \
$(abspath ../gfx/orkney24pts.c) \
$(abspath ../gfx/orkney8pts.c) \
$(abspath ../gfx/7seg32.c) \
$(abspath ../gfx/icons.c) \
$(abspath ../gfx/icon_data.c) \
$(abspath ../app/app_time.c) \
$(abspath ../app/app_hrm.c) \
$(abspath ../app/watchface.c) \
$(abspath ../app/status.c) \
$(abspath ../app/tetris.c) \
$(abspath ../ble/ble.c) \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath $(COMPONENTS)/toolchain/gcc/gcc_startup_nrf52.s)

#includes common to all targets
INC_PATHS += -I$(abspath ../config)
INC_PATHS += -I$(abspath ../gfx)
INC_PATHS += -I$(abspath ../hardware)
INC_PATHS += -I$(abspath ../app)
INC_PATHS += -I$(abspath ../ble)
INC_PATHS += -I$(abspath ../freertos/include)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/config)
#INC_PATHS += -I$(abspath $(NRF5_SDK_PATH)/examples/bsp)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/timer)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/fifo)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/fstorage/config)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/delay)
INC_PATHS += -I$(abspath $(COMPONENTS)/softdevice/s132/headers/nrf52)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/util)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/uart)
INC_PATHS += -I$(abspath $(COMPONENTS)/ble/common)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/pstorage)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/uart)
INC_PATHS += -I$(abspath $(COMPONENTS)/device)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/button)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/pwm)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/fstorage)
INC_PATHS += -I$(abspath $(COMPONENTS)/libraries/experimental_section_vars)
INC_PATHS += -I$(abspath $(COMPONENTS)/softdevice/s132/headers)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/gpiote)
INC_PATHS += -I$(abspath $(COMPONENTS)/ble/ble_services/ble_nus)
INC_PATHS += -I$(abspath $(COMPONENTS)/toolchain/CMSIS/Include)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/hal)
INC_PATHS += -I$(abspath $(COMPONENTS)/toolchain/gcc)
INC_PATHS += -I$(abspath $(COMPONENTS)/toolchain)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/common)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/clock)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/spi_master)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/wdt)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/saadc)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/ppi)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/timer)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/twi_master)
INC_PATHS += -I$(abspath $(COMPONENTS)/drivers_nrf/hal)
INC_PATHS += -I$(abspath $(COMPONENTS)/ble/ble_advertising)
INC_PATHS += -I$(abspath $(COMPONENTS)/softdevice/common/softdevice_handler)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DNRF52
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DBOARD_CUSTOM
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_53
#CFLAGS += -DNRF_LOG_USES_UART=1
CFLAGS += -DS132
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DNRF52_PAN_62
CFLAGS += -DNRF52_PAN_63
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
#CFLAGS += -Wall -Werror -O3 -g3
CFLAGS += -Wall -Og -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 
CFLAGS += -L./pah8series
CFLAGS += -DARM_MATH_CM4

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
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
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_53
#ASMFLAGS += -DNRF_LOG_USES_UART=1
ASMFLAGS += -DS132
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DNRF52_PAN_62
ASMFLAGS += -DNRF52_PAN_63
ASMFLAGS += -D__HEAP_SIZE=512
ASMFLAGS += -DARM_MATH_CM4

#LIBS = $(abspath ./pah8series.ar) 
LIBS = $(abspath ../lib/libarm_cortexM4lf_math.a)

#default target - first one defined
#default: clean application zip
default: application zip

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e application
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e zip

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	application
	@echo 	flash_softdevice

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

application: OUTPUT_FILENAME := application
application: LINKER_SCRIPT=ble_app_uart_gcc_nrf52.ld

application: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
flash: application
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex -f nrf52  --sectorerase
	nrfjprog --reset -f nrf52

## Flash softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_2.0.0_softdevice.hex
	nrfjprog --program ../../../../../../components/softdevice/s132/hex/s132_nrf52_2.0.0_softdevice.hex -f nrf52 --chiperase
	nrfjprog --reset -f nrf52
	
## Generate .zip for DFU
zip: application
	nrfutil dfu genpkg --application $(OUTPUT_BINARY_DIRECTORY)/$<.hex --application-version 0xffffffff --dev-revision 0xffff --dev-type 0xffff --sd-req 136 app_dfu_package.zip
	