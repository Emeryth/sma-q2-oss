
FLAGS = -fPIC
CFLAGS = $(INC_FLAGS) $(FLAGS) $(DIRECTIVES) --std=gnu99
LIBS = lib/libarm_cortexM4lf_math.a

DIRECTIVES = -DPB_FIELD_16BIT -DLOG_USE_COLOR -DUNITY_OUTPUT_COLOR -DDEBUG_LEVEL=1

DIRECTIVES += -DNRF52
DIRECTIVES += -DSOFTDEVICE_PRESENT
DIRECTIVES += -DBOARD_CUSTOM
DIRECTIVES += -DNRF52_PAN_12 -DNRF52_PAN_15 -DNRF52_PAN_58 -DNRF52_PAN_55 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_30
DIRECTIVES += -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_53 -DNRF52_PAN_20 -DNRF52_PAN_64 -DNRF52_PAN_62 -DNRF52_PAN_63
DIRECTIVES += -DS132
DIRECTIVES += -DCONFIG_GPIO_AS_PINRESET
DIRECTIVES += -DBLE_STACK_SUPPORT_REQD
DIRECTIVES += -DSWI_DISABLE0
DIRECTIVES += -DARM_MATH_CM4


#flags common to all targets
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Og -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
CFLAGS += -L./pah8series

# keep every function in separate section. This will allow linker to dump unused functions
TEMPLATE_PATH = $(NRF5_SDK_DIR)/components/toolchain/gcc
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
ASMFLAGS += $(DIRECTIVES)
ASMFLAGS += -D__HEAP_SIZE=512
ASMFLAGS += -DARM_MATH_CM4