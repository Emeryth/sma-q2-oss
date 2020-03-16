include conf/nrf_libs.mk

CP = cp
CD = cd
PATCH = patch
CLEANUP = rm -f
MKDIR = mkdir -p
FIND = /usr/bin/find
MV = /bin/mv
SED = /bin/sed

SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = .build/
RELEASE_DIR = release/
PATCH_DIR = patch
CONFIG_DIR = conf/
NRF5_SDK_DIR = $(LIB_DIR)/nRF5_SDK_11.0.0_89a8197
UNITY_ROOT=$(LIB_DIR)/unity
TARGET_EXTENSION=out

# all source files in lib and the explicit nordic libs down 2 directories in depth
ALL_SRCS := $(shell find $(LIB_DIR) $(NORDIC_INCS) -maxdepth 2 \( -iname "*.c" \))
# all the source files in src
ALL_SRCS += $(shell find $(SRC_DIR) \( -iname "*.c" ! -iname "*.pb.c" \))

# all include directories... searching 2 deep seems to work but requries we call a couple libs out...
ALL_INCS = $(LIB_DIR)/unity/src
ALL_INCS += $(LIB_DIR)/freertos/include
ALL_INCS := $(shell find $(LIB_DIR) $(SRC_DIR) -maxdepth 2 -type d) $(NORDIC_INCS) $(LIB_DIR)

#from the include directories generate a list of include flags
INC_DIRS := $(addprefix -I,$(ALL_INCS)) -I./$(SRC_DIR)
