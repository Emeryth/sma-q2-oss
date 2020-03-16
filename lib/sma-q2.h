/*
 * sma-q2.h
 *
 *  Created on: May 26, 2018
 *      Author: Andrzej Surowiec
 */

#ifndef CONFIG_SMA_Q2_H_
#define CONFIG_SMA_Q2_H_

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#define RX_PIN_NUMBER  25
#define TX_PIN_NUMBER  26
#define CTS_PIN_NUMBER 0
#define RTS_PIN_NUMBER 0
#define HWFC           false


#define BUTTON_BACK_PIN 9
#define BUTTON_UP_PIN 27
#define BUTTON_OK_PIN 28
#define BUTTON_DOWN_PIN 29

#define BUTTONS_MASK ((1<<BUTTON_BACK_PIN)|(1<<BUTTON_UP_PIN)|(1<<BUTTON_OK_PIN)|(1<<BUTTON_DOWN_PIN))

#define LCD_SPI_INSTANCE 2

#define LCD_CS_PIN 5
#define LCD_POWER_PIN 7
#define LCD_COM_PIN 6

#define ACCEL_INT_PIN 15
#define ACCEL_POWER_PIN 16
#define ACCEL_SDA_PIN 17
#define ACCEL_SCL_PIN 18

#define HRM_SDA_PIN 19
#define HRM_SCL_PIN 20
#define HRM_POWER_PIN 31
#define HRM_RESET_PIN 10
#define HRM_INT_PIN 22

#define CHARGER_STATUS_PIN 23
#define CHARGER_POWER_GOOD_PIN 24

#define FLASH_CS_PIN 12
#define BACKLIGHT_PIN 8
#define VIBRATION_PIN 30

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         16                                           /**< Size of timer operation queues. */

#endif /* CONFIG_SMA_Q2_H_ */
