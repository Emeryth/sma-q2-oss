/*
 * vibration.c
 *
 *  Created on: Jun 1, 2019
 *      Author: Andrzej Surowiec
 */

#include "vibration.h"
#include "app_pwm.h"
#include "sma-q2.h"
#include "FreeRTOS.h"
#include "timers.h"

vibration_state_t vibration_state;
uint8_t pwm_enabled=0;

APP_PWM_INSTANCE(PWM0, 2);

void vibration_init(void){

	app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_1CH(500L, VIBRATION_PIN);

	pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

	app_pwm_init(&PWM0, &pwm0_cfg, NULL);
//	app_pwm_enable(&PWM0);

//	app_pwm_channel_duty_set(&PWM0,0,0);

}

void vibration_step(void){


	if (vibration_state.repeat!=0){
	uint8_t duty = vibration_state.pattern.pattern[vibration_state.step];
	if (!pwm_enabled){
		app_pwm_enable(&PWM0);
		pwm_enabled=1;
	}
	app_pwm_channel_duty_set(&PWM0,0,duty);

	vibration_state.step++;

	if (vibration_state.step==VIBRATION_PATTERN_LEN){
		vibration_state.step=0;

		if (vibration_state.repeat>0){
			vibration_state.repeat--;
		}
	}
	}
	else{
		if (pwm_enabled){
			app_pwm_channel_duty_set(&PWM0,0,0);
			app_pwm_disable(&PWM0);
			pwm_enabled=0;
		}
	}

}

void vibration_set_pattern(vibration_pattern_t pattern){

	vibration_state.pattern=pattern;
	vibration_state.step=0;
	vibration_state.repeat=pattern.repeat;

}

void vibration_short(void){

	vibration_pattern_t vibr={.repeat=1,.pattern={60,0,0,0,0,0,0,0}};
	vibration_set_pattern(vibr);
}

void vibration_stop(void){
	vibration_pattern_t vibr={.repeat=0,.pattern={0,0,0,0,0,0,0,0}};
	vibration_set_pattern(vibr);
}

void vibration_notification(void){
	vibration_pattern_t vibr={.repeat=1,.pattern={100,100,0,100,100,0,0,0}};
	vibration_set_pattern(vibr);
}

void vibration_call(void){
	vibration_pattern_t vibr={.repeat=20,.pattern={100,100,100,100,0,0,0,0}};
	vibration_set_pattern(vibr);
}
