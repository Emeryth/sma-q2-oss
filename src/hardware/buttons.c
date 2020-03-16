
/*
 * buttons.c
 *
 *  Created on: Oct 20, 2018
 *      Author: Andrzej Surowiec
 */
#include "buttons.h"
#include "backlight.h"
#include "tetris.h"

QueueHandle_t button_evt_queue;

int button_pins[BUTTON_NUM]={
		BUTTON_BACK_PIN,
		BUTTON_UP_PIN,
		BUTTON_OK_PIN,
		BUTTON_DOWN_PIN
};

static void back_button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){

//	if (!nrf_drv_gpiote_in_is_set(BUTTON_UP_PIN) &&  !nrf_drv_gpiote_in_is_set(BUTTON_DOWN_PIN)){
//	if ( !nrf_drv_gpiote_in_is_set(BUTTON_DOWN_PIN)){

	if (pin==BUTTON_BACK_PIN){
	if (!nrf_gpio_pin_read(BUTTON_DOWN_PIN)){

		sd_nvic_SystemReset();

	}
	}

//	backlight_on();
//	setinput('l');
}

static void up_button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){

	backlight_on();
	setinput('t');
}

static void ok_button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){

	backlight_on();
	setinput('d');

}

static void down_button_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){

	if ( !nrf_drv_gpiote_in_is_set(BUTTON_BACK_PIN)){

		sd_nvic_SystemReset();

	}

	backlight_on();
	setinput('r');
}

void buttons_init(void) {

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

	int err_code = nrf_drv_gpiote_in_init(BUTTON_BACK_PIN, &in_config, back_button_handler);
	APP_ERROR_CHECK(err_code);
//	nrf_drv_gpiote_in_init(BUTTON_UP_PIN, &in_config, up_button_handler);
//	nrf_drv_gpiote_in_init(BUTTON_OK_PIN, &in_config, ok_button_handler);
//	nrf_drv_gpiote_in_init(BUTTON_DOWN_PIN, &in_config, down_button_handler);
	nrf_gpio_cfg_input(BUTTON_UP_PIN,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON_OK_PIN,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON_DOWN_PIN,NRF_GPIO_PIN_PULLUP);

	nrf_drv_gpiote_in_event_enable(BUTTON_BACK_PIN, true);

}


void send_button_evt(uint8_t button,uint8_t evt,int cnt){

	button_event_t bevt;

	bevt.button=button;
	bevt.press_type=evt;
	bevt.hold_counter=cnt;

	xQueueSend( button_evt_queue, ( void * )&bevt, ( TickType_t ) 0 );
}
