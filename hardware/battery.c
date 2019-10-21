/*
 * battery.c
 *
 *  Created on: May 12, 2019
 *      Author: Andrzej Surowiec
 */

#include "nrf_gpio.h"
#include "nrf_drv_saadc.h"
#include "battery.h"

nrf_saadc_value_t buffer[1];
int batt_voltage;

void saadc_callback(nrf_drv_saadc_evt_t const * p_event) {
	if (p_event->type == NRF_DRV_SAADC_EVT_DONE) {
		ret_code_t err_code;

		err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
		APP_ERROR_CHECK(err_code);

		batt_voltage = p_event->data.done.p_buffer[0];

//       	err_code = nrf_drv_saadc_sample();
		nrf_drv_saadc_uninit();
		NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear
				<< SAADC_INTENCLR_END_Pos);
		NVIC_ClearPendingIRQ(SAADC_IRQn);
	}
}

int battery_get_voltage(void) {

	// One ADC count is 14mV on battery
	// 10-bit ADC scale, 0.6V reference, 1/6 ADC gain and 1/4 resistor divider
//	return batt_voltage*1000/1024*60/100*ADC_GAIN_INV*RESISTOR_DIVIDER_INV;
	return batt_voltage * 1429 / 100;
}

uint8_t battery_get_percent(void) {

	if (batt_voltage <= 235) {
		return 0;
	} else if (batt_voltage >= 300) {
		return 100;
	}

	return (batt_voltage - 235) * 153 / 100;

}

uint8_t battery_get_charge_status(void) {

	if (nrf_gpio_pin_read(CHARGER_POWER_GOOD_PIN)) {
		return BATTERY_DISCHARGING;
	} else if (nrf_gpio_pin_read(CHARGER_POWER_GOOD_PIN)) {
		return BATTERY_CHARGED;
	} else {
		return BATTERY_CHARGING;
	}

}

void battery_init(void) {
	nrf_gpio_cfg_input(CHARGER_STATUS_PIN, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(CHARGER_POWER_GOOD_PIN, NRF_GPIO_PIN_NOPULL);
}

void saadc_init(void) {
	ret_code_t err_code;
	nrf_saadc_channel_config_t channel_config =
	NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);
	err_code = nrf_drv_saadc_init(NULL, saadc_callback);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_channel_init(0, &channel_config);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_buffer_convert(buffer, 1);
	APP_ERROR_CHECK(err_code);

	// Enable burst mode
	NRF_SAADC->CH[0].CONFIG |= 0x01000000;
}

void battery_sample(void) {
	saadc_init();
	nrf_drv_saadc_sample();
}
