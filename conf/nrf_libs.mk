#non auto-included dirs (we could increase the lib depth but the result would be bringing in a bunch of unneeded files)
NORDIC_INCS = $(NRF5_SDK_DIR)/components/drivers_nrf/config
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/ecc
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/trace
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/timer
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/scheduler
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/fds
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/fds/config
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/fifo
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/fstorage/config
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/delay
NORDIC_INCS += $(NRF5_SDK_DIR)/components/softdevice/s132/headers/nrf52
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/util
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/uart
NORDIC_INCS += $(NRF5_SDK_DIR)/components/ble/common
NORDIC_INCS += $(NRF5_SDK_DIR)/components/ble/peer_manager
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/pstorage
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/uart
NORDIC_INCS += $(NRF5_SDK_DIR)/components/device
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/button
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/pwm
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/fstorage
NORDIC_INCS += $(NRF5_SDK_DIR)/components/libraries/experimental_section_vars
NORDIC_INCS += $(NRF5_SDK_DIR)/components/softdevice/s132/headers
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/gpiote
NORDIC_INCS += $(NRF5_SDK_DIR)/components/ble/ble_services/ble_nus
NORDIC_INCS += $(NRF5_SDK_DIR)/components/toolchain/CMSIS/Include
NORDIC_INCS += $(NRF5_SDK_DIR)/components/toolchain/gcc
NORDIC_INCS += $(NRF5_SDK_DIR)/components/toolchain
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/common
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/clock
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/spi_master
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/wdt
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/saadc
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/ppi
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/rng
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/timer
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/twi_master
NORDIC_INCS += $(NRF5_SDK_DIR)/components/drivers_nrf/hal
NORDIC_INCS += $(NRF5_SDK_DIR)/components/ble/ble_advertising
NORDIC_INCS += $(NRF5_SDK_DIR)/components/softdevice/common/softdevice_handler