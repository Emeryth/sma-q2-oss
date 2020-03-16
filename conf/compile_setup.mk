include conf/flags.mk

#some files in nrf libraries are mutually exclusive... filter out unneeded / unused files
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/hal/nrf_ecb.c,$(ALL_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/pstorage/pstorage_nosd.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/pstorage/pstorage_raw.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/spi_master/spi_5W_master.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/twi_master/deprecated/twi_sw_master.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/twi_master/deprecated/twi_hw_master.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/scheduler/app_scheduler_serconn.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer_rtx.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/timer/app_timer_ble_gzll.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/libraries/uart/app_uart.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/toolchain/system_nrf51.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/components/toolchain/system_nrf51422.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/external/freertos/%,$(FILTERED_SRCS))

#take the filtered srcs and create a list of object files to generate
FILTERED_OBJECTS = $(FILTERED_SRCS:%=$(BUILD_DIR)%.o)

#generate a list of assembly object files
ASM_SOURCE_FILES  = $(abspath $(NRF5_SDK_DIR)/components/toolchain/gcc/gcc_startup_nrf52.s)
ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_OBJECTS = $(addprefix $(BUILD_DIR), $(ASM_SOURCE_FILE_NAMES:.s=.o) )

#set the vpath (default search path) for .s files
ASM_PATHS = $(NRF5_SDK_DIR)/components/toolchain/gcc
vpath %.s $(ASM_PATHS)

#the final list of objects to be build
OBJECTS = $(PB_OBJS) $(FILTERED_OBJECTS) $(ASM_OBJECTS)

#arm versions of gcc are the defaults
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar

ifeq ($(filter test,$(MAKECMDGOALS)),test)
#override defaults so tests can run on x86 host
CC = gcc
LD = ld
AR = ar
CFLAGS =  -O0 -g3 $(INC_FLAGS)
LDFLAGS = -shared

#for testing, further filter sources so we don't try to compile arm-specific stuff
#for now filter out src since it makes references back to nrf5 code
FILTERED_SRCS := $(filter-out src/main.c,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out src/ble/%,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out src/gfx/%,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out src/app/%,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out src/util/%,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out src/hardware/%,$(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/nRF5_SDK_11.0.0_89a8197/%, $(FILTERED_SRCS))
FILTERED_SRCS := $(filter-out lib/freertos/%,$(FILTERED_SRCS))

#add in unity to set of sources
FILTERED_SRCS := $(shell find $(UNITY_ROOT)/src \( -iname "*.c" \))

#overide objets to exclude nrf5 stuff
OBJECTS = $(FILTERED_OBJECTS)

endif