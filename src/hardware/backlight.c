/*
 * backlight.c
 *
 *  Created on: May 12, 2019
 *      Author: Andrzej Surowiec
 */

#include "backlight.h"
#include "FreeRTOS.h"
#include "app_pwm.h"
#include "app_timer.h"
#include "sma-q2.h"
#include "timers.h"

APP_PWM_INSTANCE(PWM1, 1);

//APP_TIMER_DEF(backlight_timer);

TimerHandle_t backlight_timer = NULL;

//static void backlight_timer_handler(void *p_context) {
//
//    app_pwm_channel_duty_set(&PWM1,0,0);
//}

void backlight_timer_callback(TimerHandle_t pxTimer)
{

    app_pwm_channel_duty_set(&PWM1, 0, 0);
    app_pwm_disable(&PWM1);
}

void backlight_init(void)
{

    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(500L, BACKLIGHT_PIN);

    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

    app_pwm_init(&PWM1, &pwm1_cfg, NULL);
    //	app_pwm_enable(&PWM1);

    //	app_pwm_channel_duty_set(&PWM1,0,0);

    backlight_timer = xTimerCreate("backlight", BACKLIGHT_TIME, pdFALSE, 0, backlight_timer_callback);

    //	app_timer_create(&backlight_timer, APP_TIMER_MODE_SINGLE_SHOT, backlight_timer_handler);
}

void backlight_on(void)
{

    app_pwm_enable(&PWM1);
    app_pwm_channel_duty_set(&PWM1, 0, BACKLIGHT_INTENSITY);
    //	app_timer_start(backlight_timer,APP_TIMER_TICKS(3000, 0),NULL);

    xTimerReset(backlight_timer, portMAX_DELAY);
}
