/*
 * accel.c
 *
 *  Created on: Jun 12, 2019
 *      Author: Andrzej Surowiec
 */

#include "accel.h"
#include "FreeRTOS.h"
#include "kx022_registers.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "sma-q2.h"
#include "task.h"

static const nrf_drv_twi_t m_twi_kx022 = NRF_DRV_TWI_INSTANCE(1);
static uint8_t txbuf[8];
static uint8_t rxbuf[8];

void accel_init(void)
{

    nrf_gpio_cfg_output(ACCEL_POWER_PIN);

    nrf_gpio_pin_set(ACCEL_POWER_PIN);

    vTaskDelay(200);

    nrf_drv_twi_init(&m_twi_kx022, NULL, NULL, NULL);
    nrf_drv_twi_enable(&m_twi_kx022);

    txbuf[0] = KX022_WHO_AM_I;
    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 1, true);
    nrf_drv_twi_rx(&m_twi_kx022, KX022_ADDR, rxbuf, 1);

    if (rxbuf[0] != KX022_WHO_AM_I_WIA_ID) {
        /* chip is not responding */
        return;
    }

    txbuf[0] = KX022_CNTL1;
    txbuf[1] = 0;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    txbuf[0] = KX022_INC1;
    txbuf[1] = KX022_INC1_IEN1 | KX022_INC1_IEA1;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    txbuf[0] = KX022_INC3;
    txbuf[1] = KX022_INC3_TFDM | KX022_INC3_TFUM;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    txbuf[0] = KX022_INC4;
    txbuf[1] = KX022_INC4_TPI1 | KX022_INC4_TDTI1;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    txbuf[0] = KX022_TILT_TIMER;
    txbuf[1] = 10;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    txbuf[0] = KX022_CNTL1;
    txbuf[1] = KX022_CNTL1_TDTE | KX022_CNTL1_TPE | KX022_CNTL1_PC1;

    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 2, false);

    nrf_drv_twi_uninit(&m_twi_kx022);
    // Workaround for anomaly 89
    *(volatile uint32_t*)0x40004FFC = 0;
    *(volatile uint32_t*)0x40004FFC;
    *(volatile uint32_t*)0x40004FFC = 1;

    vTaskDelay(50);
}

int accel_check(void)
{

    int event = 0;

    nrf_drv_twi_init(&m_twi_kx022, NULL, NULL, NULL);
    nrf_drv_twi_enable(&m_twi_kx022);

    // Check interrupt
    txbuf[0] = KX022_STATUS_REG;
    nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 1, true);
    nrf_drv_twi_rx(&m_twi_kx022, KX022_ADDR, rxbuf, 1);

    if (rxbuf[0] & KX022_STATUS_REG_INT) {

        // Check what caused the interrupt
        txbuf[0] = KX022_TSCP;
        nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 1, true);
        nrf_drv_twi_rx(&m_twi_kx022, KX022_ADDR, rxbuf, 4);

        if (rxbuf[3] & KX022_INS2_TPS) {

            if (rxbuf[0] & KX022_TSPP_FD) {
                event = 1;
            }

        } else if (rxbuf[3] & (KX022_INS2_TDTS_DOUBLE)) {

            event = 1;
        }

        // Clear interrupt
        txbuf[0] = KX022_INT_REL;
        nrf_drv_twi_tx(&m_twi_kx022, KX022_ADDR, txbuf, 1, true);
        nrf_drv_twi_rx(&m_twi_kx022, KX022_ADDR, rxbuf, 1);
    }

    nrf_drv_twi_uninit(&m_twi_kx022);
    // Workaround for anomaly 89
    *(volatile uint32_t*)0x40004FFC = 0;
    *(volatile uint32_t*)0x40004FFC;
    *(volatile uint32_t*)0x40004FFC = 1;

    return event;
}
