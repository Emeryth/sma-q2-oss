/*
 * pah8002.c
 *
 *  Created on: Jun 13, 2019
 *      Author: Andrzej Surowiec
 */

#include "pah8002.h"
#include "pah8series_api_c.h"
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sma-q2.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_nvic.h"
#include "app_hrm.h"

//uint8_t pah8002_buffer[13000];

uint8_t pah8002_write_reg(uint8_t addr, uint8_t data);
uint8_t pah8002_read_reg(uint8_t addr, uint8_t *data);
uint8_t pah8002_burst_read_reg(uint8_t addr, uint8_t *data, uint32_t rx_size);

const uint8_t init_ppg_register_array[][2] = { { 0x7f, 0x01 }, { 0xE6, 0xC8 }, {
		0xE7, 0x00 }, { 0x07, 0x01 }, { 0xAE, 0x06 }, { 0xAF, 0x07 }, { 0x4D,
		0x00 }, { 0xBA, 0x7C }, { 0xBB, 0x7C }, { 0xBC, 0x7C }, { 0xBD, 0x06 },
		{ 0xBE, 0x06 }, { 0xBF, 0x06 }, { 0xB1, 0x06 }, { 0xB2, 0x06 }, { 0xB3,
				0x06 }, { 0x6A, 0x00 }, { 0x6B, 0x01 }, { 0x6C, 0x10 }, { 0x6D,
				0x10 }, { 0x7A, 0x00 }, { 0x6F, 0x10 }, { 0x7F, 0x00 }, { 0x08,
				0xFF }, { 0x09, 0x03 }, { 0x4F, 0x0C }, { 0xE6, 0x07 }, { 0x8C,
				0x00 }, { 0xAE, 0x01 }, { 0xD0, 0x01 }, { 0x8E, 0x00 }, { 0xD2,
				0x01 }, { 0xB0, 0x01 }, { 0x27, 0x80 }, { 0x28, 0x12 }, { 0x35,
				0xC0 }, { 0x36, 0x12 }, { 0x37, 0xC0 }, { 0x38, 0x12 }, { 0x39,
				0xC0 }, { 0x3A, 0x12 }, { 0xDE, 0x00 }, { 0xD9, 0x01 }, { 0xDD,
				0x04 }, { 0x3B, 0x01 }, { 0x3C, 0x15 }, { 0x3D, 0x15 }, { 0x47,
				0x01 }, { 0x48, 0x01 }, { 0x49, 0x01 }, { 0x4A, 0x01 }, { 0x4B,
				0x00 }, { 0x4C, 0x00 }, { 0x4D, 0x00 }, { 0x16, 0x00 }, { 0x13,
				0x01 }, { 0x14, 0x01 }, { 0x15, 0x01 }, { 0x50, 0x00 }, { 0x59,
				0x00 }, { 0x56, 0x00 }, { 0x57, 0x00 }, { 0x6B, 0x01 }, { 0x6C,
				0x00 }, { 0x8F, 0x01 }, { 0xB1, 0x01 }, { 0x3E, 0x02 }, { 0x3F,
				0x04 }, { 0x40, 0x04 }, { 0x0D, 0x78 }, { 0x0E, 0x00 }, { 0x0F,
				0xF0 }, { 0x10, 0x00 }, { 0x11, 0xF0 }, { 0x12, 0x00 }, { 0x6D,
				0xF0 }, { 0x6E, 0x00 }, { 0x6F, 0x00 }, { 0x70, 0x02 }, { 0x71,
				0x10 }, { 0x72, 0x00 }, { 0x77, 0x00 }, { 0x78, 0x0C }, { 0x79,
				0x00 }, { 0x7A, 0x08 }, { 0x7B, 0x00 }, { 0x7C, 0x0B }, { 0x7D,
				0x00 }, { 0x7E, 0x09 }, { 0x80, 0x00 }, { 0x81, 0x0D }, { 0x82,
				0x00 }, { 0x83, 0x07 }, { 0x85, 0x01 }, { 0x90, 0xF0 }, { 0x91,
				0x00 }, { 0x92, 0x20 }, { 0x93, 0x12 }, { 0x94, 0x10 }, { 0x95,
				0x00 }, { 0x9A, 0x00 }, { 0x9B, 0x0C }, { 0x9C, 0x00 }, { 0x9D,
				0x08 }, { 0x9E, 0x00 }, { 0x9F, 0x0B }, { 0xA0, 0x00 }, { 0xA1,
				0x09 }, { 0xA2, 0x00 }, { 0xA3, 0x0D }, { 0xA4, 0x00 }, { 0xA5,
				0x07 }, { 0xA7, 0x01 }, { 0xB2, 0xF0 }, { 0xB3, 0x00 }, { 0xB4,
				0x20 }, { 0xB5, 0x12 }, { 0xB6, 0x10 }, { 0xB7, 0x00 }, { 0xBC,
				0x00 }, { 0xBD, 0x0C }, { 0xBE, 0x00 }, { 0xBF, 0x08 }, { 0xC0,
				0x00 }, { 0xC1, 0x0B }, { 0xC2, 0x00 }, { 0xC3, 0x09 }, { 0xC4,
				0x00 }, { 0xC5, 0x0D }, { 0xC6, 0x00 }, { 0xC7, 0x07 }, { 0xC9,
				0x01 }, { 0x7F, 0x02 },
		//switch to bank2
		{ 0x17, 0x00 }, { 0x18, 0x00 }, { 0x1B, 0x01 }, { 0x1C, 0x01 }, { 0x25,
				0x02 }, { 0x29, 0x00 }, { 0x2d, 0x01 }, { 0x4F, 0x10 }, { 0x66,
				0x01 }, { 0x67, 0x01 }, { 0x68, 0x01 }, { 0x69, 0x01 }, { 0x6A,
				0x01 },{ 0x7a, 0x01 },
//{0x6B, 0x01},
//{0x6C, 0x01},
		{ 0x6D, 0x01 }, { 0x6E, 0x01 }, { 0x70, 0x01 }, { 0x7B, 0xFF }, { 0x7F,
				0x01 },
		//switch to bank1
		{ 0xA2, 0x40 }, { 0x7C, 0x01 }, { 0x4F, 0x07 }, { 0x3F, 0x04 }, { 0x0C,
				0x05 }, { 0x4D, 0x05 }, { 0x52, 0x05 }, { 0x86, 0x50 }, { 0x92,
				0x1C }, { 0x98, 0x1D }, { 0x9A, 0x42 }, { 0x81, 0x01 }, { 0x3B,
				0x00 }, { 0xEA, 0xC9 }, { 0xA4, 0x50 }, { 0xA5, 0x00 }, { 0xA6,
				0x52 }, { 0xA7, 0x00 }, { 0xA8, 0x53 }, { 0xA9, 0x00 }, { 0xD6,
				0x40 }, { 0xD7, 0x06 }, { 0xD8, 0x01 }, { 0xD9, 0x00 }, { 0xDA,
				0x11 }, { 0xDB, 0x00 }, { 0xDC, 0x84 }, { 0xDD, 0x02 }, { 0xDE,
				0x85 }, { 0xDF, 0x02 }, { 0xE0, 0x3F }, { 0xE1, 0x06 }, };
#define INIT_PPG_REG_ARRAY_SIZE (sizeof(init_ppg_register_array)/sizeof(init_ppg_register_array[0]))


const uint8_t suspend_register_array[][2] = { { 0x7f, 0x01 }, { 0x09, 0x01 }, {
		0x23, 0x01 }, { 0xB4, 0x01 }, { 0xB7, 0x01 }, { 0xE6, 0xC8 }, { 0xE7,
		0x00 }, { 0xF1, 0x00 }, { 0x07, 0x01 }, { 0xAE, 0x06 }, { 0xAF, 0x07 },
		{ 0xBA, 0x7C }, { 0x6C, 0x10 }, { 0x6D, 0x10 }, { 0x7A, 0x00 }, { 0x6F,
				0x10 }, { 0x7F, 0x00 }, { 0x08, 0xFF }, { 0x09, 0x03 }, { 0xD6,
				0x01 }, { 0x5C, 0x00 }, { 0x5D, 0x05 }, { 0x60, 0x00 }, { 0x61,
				0x03 }, { 0x64, 0x05 }, { 0x65, 0x05 }, { 0x35, 0x80 }, { 0x36,
				0x02 }, { 0x8C, 0x00 }, { 0x8E, 0x00 }, { 0xDE, 0x00 }, { 0xD9,
				0x01 }, { 0xDD, 0x04 }, { 0x3B, 0x01 }, { 0x47, 0x01 }, { 0x48,
				0x00 }, { 0x49, 0x00 }, { 0x4A, 0x00 }, { 0x4D, 0x00 }, { 0x16,
				0x00 }, { 0x13, 0x01 }, { 0x14, 0x01 }, { 0x15, 0x01 }, { 0x50,
				0x01 }, { 0x51, 0x01 }, { 0x59, 0x00 }, { 0x57, 0x00 }, { 0x6B,
				0x00 }, { 0x6C, 0x00 }, { 0x3E, 0x00 }, { 0x43, 0x00 }, { 0x0D,
				0x78 }, { 0x0E, 0x00 }, { 0x7F, 0x02 },
		//switch to bank2
		{ 0x17, 0x00 }, { 0x18, 0x00 }, { 0x1B, 0x01 }, { 0x1C, 0x01 }, { 0x1F,
				0x00 }, { 0x29, 0x00 }, { 0x2d, 0x01 }, { 0x2B, 0x00 }, { 0x2C,
				0x00 }, { 0x31, 0x00 }, { 0x4F, 0x10 }, { 0x66, 0x01 }, { 0x67,
				0x01 }, { 0x68, 0x01 }, { 0x69, 0x01 }, { 0x6A, 0x01 }, { 0x6B,
				0x01 }, { 0x6C, 0x01 }, { 0x6D, 0x01 }, { 0x6E, 0x01 }, { 0x6F,
				0x01 }, { 0x70, 0x01 }, { 0x74, 0x00 }, { 0x76, 0x01 }, { 0x78,
				0x01 }, { 0x7A, 0x01 }, { 0x7B, 0xFF }, { 0x8D, 0x01 }, { 0x8F,
				0x01 }, { 0x92, 0x00 }, { 0x7F, 0x01 },
		//switch to bank1
		{ 0xA2, 0x40 }, { 0x7C, 0x01 }, { 0x4C, 0x01 }, { 0x4F, 0x07 }, { 0x3F,
				0x04 }, { 0x0C, 0x05 }, { 0x4D, 0x05 }, { 0x52, 0x05 }, { 0x86,
				0x50 }, { 0x92, 0x1C }, { 0x98, 0x1D }, { 0x9A, 0x42 }, { 0x81,
				0x01 }, { 0x3B, 0x00 }, { 0xEA, 0xC9 }, { 0xA4, 0x50 }, { 0xA5,
				0x00 }, { 0xA6, 0x52 }, { 0xA7, 0x00 }, { 0xA8, 0x53 }, { 0xA9,
				0x00 }, { 0xD6, 0xFF }, { 0xD7, 0x1F }, { 0xD8, 0x01 }, { 0xD9,
				0x00 }, { 0xDA, 0x10 }, { 0xDB, 0x00 }, { 0xDC, 0x13 }, { 0xDD,
				0x00 }, { 0xDE, 0x14 }, { 0xDF, 0x00 }, { 0xE0, 0xFE }, { 0xE1,
				0x1F }, { 0x7F, 0x01 },
		//switch to bank1
		{ 0xd5, 0x01 }, };
#define SUSPEND_REG_ARRAY_SIZE (sizeof(suspend_register_array)/sizeof(suspend_register_array[0]))


#define NF_PPG_CHANNEL 3
#define NF_PPG_PER_CHANNEL 20
#define NF_MEMS_MAX 25

volatile float _hr = 0.0 ;
volatile uint8_t   ret_status;
volatile uint8_t init_result;
//TaskHandle_t  hrm_task;


float __hardfp_sqrtf(float x) {

	return sqrtf(x);
}

float __hardfp_expf(float x) {
	return expf(x);
}
float __hardfp_log10f(float x) {
	return log10f(x);
}

#define I2C_ID_PAH8002  0x15  //I2C 7-bit ID

static const nrf_drv_twi_t m_twi_pah8002 = NRF_DRV_TWI_INSTANCE(0);
uint8_t txbuf[8];
uint8_t rxbuf[8];



static uint8_t pah8002_sw_reset() {
	uint8_t data;
	//debug_printf(">>>pah8002_sw_reset\r\n");
	pah8002_wakeup();
	if (0 != pah8002_write_reg(0x7f, 0x00)) {
		goto RTN;
	}
	if (0 != pah8002_read_reg(0, &data)) {
		goto RTN;
	}
	//debug_printf("ID=%d\r\n", data);
	if (data != 0x02) {
		goto RTN;
	}
	if (0 != pah8002_write_reg(0xe1, 0)) //write 0 to trigger Software Reset
			{
		goto RTN;
		//delay               5ms
	}
//	delay(5);
	vTaskDelay(5);
	//debug_printf("<<<pah8002_sw_reset\r\n");
	return true;
	RTN: return false;
}

static uint8_t pah8002_start() {
	uint8_t data = 0;
	int samples_per_read = HEART_RATE_MODE_SAMPLES_PER_READ;
	//debug_printf(">>>pah8002_start  \r\n");
	pah8002_wakeup();
	if (0 != pah8002_write_reg(0x7f, 0x01)) {
		goto RTN;
	} else if (0 != pah8002_write_reg(0xea, (samples_per_read + 1))) {
		goto RTN;
	} else if (0 != pah8002_write_reg(0xd5, 1)) //TG enable. REQTIMER_ENABLE
			{
		goto RTN;
	} else if (0 != pah8002_read_reg(0xd5, &data)) //TG enable. REQTIMER_ENABLE
			{
		goto RTN;
	}
	pah8002_check();
	//debug_printf("<<<  pah8002_start  %d\r\n", data);
	return true;
	RTN: return false;
}

//static uint8_t pah8002_touch_mode_init() {
//	int i = 0;
//	//debug_printf(">>> pah8002_touch_mode_init \r\n");
//	pah8002_wakeup();
//	for (i = 0; i < INIT_TOUCH_REG_ARRAY_SIZE; i++) {
//		if (pah8002_write_reg(init_touch_register_array[i][0],
//				init_touch_register_array[i][1]) != 0) {
//			goto RTN;
//		}
//	}
//	//debug_printf("<<< pah8002_touch_mode_init \r\n");
//	return true;
//	RTN: return false;
//
//}
static uint8_t pah8002_normal_mode_init() {
	int i = 0;
	//debug_printf(">>> pah8002_normal_mode_init \r\n");
	pah8002_wakeup();
	for (i = 0; i < INIT_PPG_REG_ARRAY_SIZE; i++) {
		if (pah8002_write_reg(init_ppg_register_array[i][0],
				init_ppg_register_array[i][1]) != 0) {
			goto RTN;
		}
	}
	//debug_printf("<<< pah8002_normal_mode_init \r\n");
	return true;
	RTN: return false;
}

//static uint8_t pah8002_stress_mode_init() {
//	int i = 0;
//	//debug_printf(">>> pah8002_stress_mode_init \r\n");
//	pah8002_wakeup();
//	for (i = 0; i < INIT_STRESS_REG_ARRAY_SIZE; i++) {
//		if (pah8002_write_reg(init_stress_register_array[i][0],
//				init_stress_register_array[i][1]) != 0) {
//			goto RTN;
//		}
//	}
//	//debug_printf("<<< pah8002_stress_mode_init \r\n");
//	return true;
//	RTN: return false;
//}

static uint8_t pah8002_get_touch_flag_ppg_mode() {
	static uint8_t touch_sts_output = 1;
	int32_t *s = (int32_t *) pah8002_ppg_data;
	int32_t ch0;
	int32_t ch1;
	int64_t ir_rawdata;
	int i;
	static int touch_cnt = 0, no_touch_cnt = 0;

#define TouchDetection_Upper_TH (600)
#define TouchDetection_Lower_TH (512)
#define TouchDetection_Count_TH  (3)    //(3+1)*50ms  = 200ms
#define NoTouchDetection_Count_TH (3) //(3+1)*50ms = 200ms

	for (i = 0; i < HEART_RATE_MODE_SAMPLES_PER_READ; i += TOTAL_CHANNELS) {
		ch0 = *s;
		ch1 = *(s + 1);
		ir_rawdata = ch0 - ch1;
		ir_rawdata = (ir_rawdata * _ir_dac * _ir_expo) >> 20;
		if (ir_rawdata > TouchDetection_Upper_TH) {
			touch_cnt++;
			no_touch_cnt = 0;
		} else if (ir_rawdata < TouchDetection_Lower_TH) {
			no_touch_cnt++;
			touch_cnt = 0;
		} else {
			touch_cnt = 0;
			no_touch_cnt = 0;
		}
		s += TOTAL_CHANNELS;
	}
	if (touch_cnt > TouchDetection_Count_TH) {
		touch_sts_output = 1;
	} else if (no_touch_cnt > NoTouchDetection_Count_TH) {
		touch_sts_output = 0;
	}
	//debug_printf("<<< pah8002_get_touch_flag_ppg_mode %d, %d\n", touch_cnt, no_touch_cnt);
	//debug_printf("<<< pah8002_get_touch_flag_ppg_mode %d\n", touch_sts_output);
	return touch_sts_output;
}

uint8_t pah8002_enter_normal_mode() {
	//debug_printf(">>> pah8002_enter_normal_mode\r\n");
	if (_mode == NORMAL_MODE)
		return true;
	//1. software reset
	if (!pah8002_sw_reset())
		goto RTN;
	//2. load registers for normal mode
	if (!pah8002_normal_mode_init())
		goto RTN;
	pah8002_write_reg(0x7f, 0x00);               //Bank0
	pah8002_read_reg(0x0D, &_ir_expo); // IR Exposure Time
	pah8002_write_reg(0x7f, 0x01);               //Bank1
	pah8002_read_reg(0xBA, &_ir_dac); //IR Led DAC
	//3.  enable               sensor
	if (!pah8002_start())
		goto RTN;
	_mode = NORMAL_MODE;
	//debug_printf("<<< pah8002_enter_normal_mode ir_dac %x, ir_expo %x\r\n", _ir_dac, _ir_expo);
	return true;
	RTN: return false;
}

static uint8_t pah8002_enter_stress_mode() {
	//debug_printf(">>> pah8002_enter_stress_mode\r\n");
	if (_mode == STRESS_MODE)
		return true;
	//1.               software               reset
	if (!pah8002_sw_reset())
		goto RTN;
	//2. load registers for normal mode
	if (!pah8002_stress_mode_init())
		goto RTN;
	pah8002_write_reg(0x7f, 0x00);               //Bank0
	pah8002_read_reg(0x0D, &_ir_expo); // IR Exposure Time
	pah8002_write_reg(0x7f, 0x01);               //Bank1
	pah8002_read_reg(0xBA, &_ir_dac); //IR Led DAC
	//3.               enable               sensor
	if (!pah8002_start())
		goto RTN;
	_mode = STRESS_MODE;
	//debug_printf("<<< pah8002_enter_stress_mode \r\n");
	return true;
	RTN: return false;
}

static uint8_t pah8002_enter_touch_mode() {
	//debug_printf(">>> pah8002_enter_touch_mode\r\n");
	if (_mode == TOUCH_MODE)
		return true;
	//1.               software               reset
	if (!pah8002_sw_reset())
		goto RTN;
	//2. load registers for touch mode
	if (!pah8002_touch_mode_init())
		goto RTN;
	//3.               enable               sensor
	if (!pah8002_start())
		goto RTN;
	_mode = TOUCH_MODE;
	//debug_printf("<<< pah8002_enter_touch_mode\r\n");
	return true;
	RTN: return false;
}

static uint8_t pah8002_get_touch_flag(uint8_t *touch_flag) {
	//debug_printf(">>> pah8002_touch_status \r\n");
	pah8002_wakeup();
	if (0 != pah8002_write_reg(0x7f, 0x02)) {
		goto RTN;
	} else if (0 != pah8002_read_reg(0x45, touch_flag)) //
			{
		goto RTN;
	}
	//debug_printf("<<< pah8002_touch_status %d\r\n", *touch_flag);
	return true;
	RTN: return false;
}

static int pah8002_wakeup() {
	volatile int retry = 0;
	int success = 0;
	uint8_t data = 0;
	pah8002_read_reg(0, &data);
	pah8002_read_reg(0, &data);
	do {
		pah8002_write_reg(0x7f, 0x00);
		pah8002_read_reg(0, &data);
		if (data == 0x02)
			success++;
		else
			success = 0;
		if (success >= 2)
			break;
		retry++;
	} while (retry < 20);

	if (_chip_id == 0) {
		pah8002_read_reg(0x02, &data);
		_chip_id = data & 0xF0;
		if (_chip_id != 0xD0) {
			//debug_printf("Not support anymore\r\n");
			while (1) {
			};
		}
	}
	pah8002_write_reg(0x7f, 0x02);
	pah8002_write_reg(0x70, 0x00);
	//debug_printf("pah8002_wakeup retry %d \r\n", retry);
	return retry;
}

static int pah8002_check() {
	int retry = 0;
	int success = 0;
	uint8_t data = 0;
	uint8_t b1_0xd5 = 0;
	uint8_t b1_0xe6 = 0;
	pah8002_read_reg(0, &data);
	pah8002_read_reg(0, &data);
	do {
		pah8002_write_reg(0x7f, 0x00);
		pah8002_read_reg(0, &data);
		if (data == 0x02)
			success++;
		else
			success = 0;
		if (success >= 2)
			break;
		retry++;
	} while (retry < 20);
	pah8002_write_reg(0x7f, 0x01);

	pah8002_read_reg(0xd5, &b1_0xd5);
	pah8002_read_reg(0xe6, &b1_0xe6);
	//debug_printf("pah8002_check retry %d \r\n", retry);
	if (b1_0xd5 != 1)
		//debug_printf("pah8002_check error  Bank1 0xD5 0x%x \r\n", b1_0xd5);
		if (b1_0xe6 != 0xC8)
			//debug_printf("pah8002_check error  Bank1 0xE6 0x%x \r\n", b1_0xe6);
			return retry;
}

uint8_t pah8002_enter_suspend_mode() {
	int i = 0;
	//debug_printf("pah8002_enter_suspend_mode");
	pah8002_sw_reset();
	for (i = 0; i < SUSPEND_REG_ARRAY_SIZE; i++) {
		if (pah8002_write_reg(suspend_register_array[i][0],
				suspend_register_array[i][1]) != 0) {
			return false;
		}
	}
	_mode = SUSPEND_MODE;
	pah8002_check();
	return true;
}

static uint8_t _pah8002_task() {
	uint8_t cks[4];
	uint8_t int_req = 0;
	//debug_printf(">>> pah8002_task\n");
	pah8002_wakeup();
	if (0 != pah8002_write_reg(0x7f, 0x02)) {

	} else if (0 != pah8002_read_reg(0x73, &int_req)) {

	} else {
		if ((int_req & 0x04) != 0) {   //overflow
			while (1)
				;
		}
		if ((int_req & 0x02) != 0) { //touch
									 //debug_printf("touch               interrupt\n");
		}
		if ((int_req & 0x08) != 0) {
			//overflow
			while (1)
				;
		}
		if ((int_req & 0x01) != 0) {
			int samples_per_read = HEART_RATE_MODE_SAMPLES_PER_READ;
			//debug_printf("FIFO               interrupt\n");
			//pah8002_get_touch_flag(&state->pah8002_touch_flag);
			if (0 != pah8002_write_reg(0x7f, 0x03)) {

			} else if (0
					!= pah8002_burst_read_reg(0, pah8002_ppg_data,
							samples_per_read * 4)) {

			} else if (0 != pah8002_write_reg(0x7f, 0x02)) {

			} else if (0 != pah8002_burst_read_reg(0x80, cks, 4)) {

			} else if (0 != pah8002_write_reg(0x75, 0x01)) //read fifo first, then clear SRAM FIFO interrupt
					{
			} else if (0 != pah8002_write_reg(0x75, 0x00)) {

			} else {
				uint32_t *s = (uint32_t *) pah8002_ppg_data;
				uint32_t cks_cal = *s;
				uint32_t cks_rx = *((uint32_t *) cks);
				uint32_t i;      //checksum    compare
				for (i = 1; i < samples_per_read; i++) {
					cks_cal = cks_cal ^ (*(s + i));
				}
				if (cks_cal != cks_rx) {
					//debug_printf("checksum error\r\n");
				} else {
					//debug_printf("checksum  OK %d\r\n", cks_cal);
				}
				_touch_flag = pah8002_get_touch_flag_ppg_mode();
			}
		} else {
			//debug_printf("not fifo  interrupt%d\r\n",  int_req);
		}
	}
	//debug_printf("<<< pah8002_task\n");
	return true;
}

//static uint8_t pah8002_normal_long_et_mode_init() {
//	int i = 0;
//	//debug_printf(">>>  pah8002_normal_long_et_mode_init \r\n");
//	pah8002_wakeup();
//	for (i = 0; i < INIT_PPG_LONG_REG_ARRAY_SIZE; i++) {
//		if (pah8002_write_reg(init_ppg_long_register_array[i][0],
//				init_ppg_long_register_array[i][1]) != 0) {
//			goto RTN;
//		}
//	}
//	//debug_printf("<<< pah8002_normal_long_et_mode_init \r\n");
//	return true;
//	RTN: return false;
//}

static uint8_t pah8002_enter_normal_long_et_mode() {
	//debug_printf(">>>               pah8002_enter_normal_long_et_mode\r\n");
	if (_mode == NORMAL_LONG_ET_MODE)
		return true;    //1.   software  reset
	if (!pah8002_sw_reset())
		goto RTN;
	//2. load registers for normal mode
	if (!pah8002_normal_long_et_mode_init())
		goto RTN;
	pah8002_write_reg(0x7f, 0x00);     //Bank0
	pah8002_read_reg(0x0D, &_ir_expo); // IR Exposure Time
	pah8002_write_reg(0x7f, 0x01);     //Bank1
	pah8002_read_reg(0xBA, &_ir_dac); //IR Led DAC
	//3.  enable sensor
	if (!pah8002_start())
		goto RTN;
	_mode = NORMAL_LONG_ET_MODE;
	//debug_printf("<<< pah8002_enter_normal_long_et_mode ir_dac %x, ir_expo %x\r\n", _ir_dac, _ir_expo);
	return true;
	RTN: return false;
}

static void pah8002_dyn_switch_ppg_mode() {
	uint8_t b2a4, b2a5;
	uint16_t value;
	pah8002_wakeup();
	pah8002_write_reg(0x7F, 0x02);
	pah8002_read_reg(0xa4, &b2a4);
	pah8002_read_reg(0xa5, &b2a5);
	value = b2a5;
	value <<= 8;
	value += b2a4;
	if (value > 4639) {
		pah8002_enter_normal_long_et_mode();
	}
}

uint32_t ulNotifiedValue;

volatile int twi_ready=0;

void twi_event_handler(const nrf_drv_twi_evt_t * p_event, void * p_context) {
//	if ((p_event->type == NRF_DRV_TWI_EVT_DONE)) {

		twi_ready=p_event->type;

//		BaseType_t xHigherPriorityTaskWoken;
//		xHigherPriorityTaskWoken = pdFALSE;
//		vTaskNotifyGiveFromISR(hrm_task, &xHigherPriorityTaskWoken);
//		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

//	}
}


uint8_t pah8002_write_reg(uint8_t addr, uint8_t data) {
	txbuf[0] = addr;
	txbuf[1] = data;

	twi_ready=-1;
	nrf_drv_twi_xfer_desc_t xfer = NRF_DRV_TWI_XFER_DESC_TX(I2C_ID_PAH8002,txbuf,2);
	ret_code_t ret = nrf_drv_twi_xfer(&m_twi_pah8002, &xfer, 0);

	if (ret!=NRF_SUCCESS){
		return ret;
	}
	while(twi_ready==-1)
	{};

//	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

	return twi_ready;
}

uint8_t pah8002_read_reg(uint8_t addr, uint8_t *data) {
	txbuf[0] = addr;

	twi_ready=-1;
	nrf_drv_twi_xfer_desc_t xfer = NRF_DRV_TWI_XFER_DESC_TXRX(I2C_ID_PAH8002, txbuf, 1, data, 1);
	ret_code_t ret = nrf_drv_twi_xfer(&m_twi_pah8002, &xfer, NRF_DRV_TWI_FLAG_TX_NO_STOP);

	if (ret!=NRF_SUCCESS){
		return ret;
	}

	while(twi_ready==-1)
	{};

//	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
	return twi_ready;
}

uint8_t pah8002_burst_read_reg(uint8_t addr, uint8_t *data, uint32_t rx_size) {
	txbuf[0] = addr;

	uint8_t counter=0;

	twi_ready=-1;
	nrf_drv_twi_xfer_desc_t xfer = NRF_DRV_TWI_XFER_DESC_TXRX(I2C_ID_PAH8002, txbuf, 1, data, rx_size);
	ret_code_t ret = nrf_drv_twi_xfer(&m_twi_pah8002, &xfer, NRF_DRV_TWI_FLAG_TX_NO_STOP);

	if (ret!=NRF_SUCCESS){
		return ret;
	}

	while(twi_ready==-1)
	{
		if (rx_size>20){
			vTaskDelay(10);
		}
	};

//	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
	return twi_ready;
}

static void data_convert_4ch_to_3ch(uint32_t *pdata, uint32_t len) {
  uint32_t i = 0, j = 0;
  for (i = 0, j = 2; j < len; i += 3, j += 4)  {
    *(pdata + i + 1)   =  *(pdata + j);
    *(pdata + i + 2) =  *(pdata + j + 1);
  }
}

void pah8002_task(void) {
  uint8_t   ret;

  uint32_t  sys_tick;
  if (_pah8002_interrupt == 1)  {
    _pah8002_interrupt   =   0;
    if (_mode  == TOUCH_MODE)  {
      pah8002_enter_normal_mode();
//      _timestamp = get_sys_tick();
//      accelerometer_start();
    } else if (_mode == NORMAL_MODE || _mode == NORMAL_LONG_ET_MODE) {
      _pah8002_task();
//      pah8002_dyn_switch_ppg_mode();

#ifdef PPG_MODE_ONLY

#else
      if (_touch_flag  ==  0)  {
        pah8002_enter_touch_mode();
		accelerometer_stop();
      }
#endif
      //process algorithm
#ifdef MEMS_ZERO
#else
      accelerometer_get_fifo(&_pah8002_data.mems_data, &_pah8002_data.nf_mems);
#endif
//      sys_tick = get_sys_tick();
//      _pah8002_data.time = sys_tick - _timestamp;
//      _timestamp  =  sys_tick;
      _pah8002_data.time = 1000;
      _pah8002_data.touch_flag =  _touch_flag;

      if (_touch_flag){
      hrm_add_samples((int32_t  *)pah8002_ppg_data);
      }
//      data_convert_4ch_to_3ch((uint32_t  *)pah8002_ppg_data,  HEART_RATE_MODE_SAMPLES_PER_READ);
      // log 3ch   ppg_data  before pah8002_entrance()
//      ret  =   pah8series_entrance(&_pah8002_data);
//      ret_status = ret;
//      if ((ret & 0x0f) != 0)    {
//        switch (ret) //check error status
//        { case MSG_ALG_NOT_OPEN:
//            //debug_printf("Algorithm is not initialized.\r\n");
//            break;
//          case MSG_MEMS_LEN_TOO_SHORT:
//            //debug_printf("MEMS data   length is  shorter than PPG data  length.\r\n");
//            break;
//          case MSG_NO_TOUCH:
//            //debug_printf("PPG  is    no touch.\r\n");
//            break;
//          case MSG_PPG_LEN_TOO_SHORT:
//            //debug_printf("PPGdata  length   is  too  short.\r\n");
//            break;
//          case MSG_FRAME_LOSS:
//            //debug_printf("Frame  count   is  not  continuous.\r\n");
//            break;
//        }
//      }
//      if ((ret & 0xf0) == MSG_HR_READY)    {
//    	  pah8series_get_hr(&_hr)               ;
//        //debug_printf("HR  =  %d\r\n",  (int)(hr));
//      }
      _pah8002_data.frame_count++;
    }
  }
}

uint32_t pah8002_get_status(void){
	return ret_status|(_touch_flag<<8);
}

float pah8002_hr(void){

	return _hr;

}



static void hrm_int_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){

	if (pin==HRM_INT_PIN){

	if (_mode == NORMAL_MODE ){
	_pah8002_interrupt =1;
	}

	}

//	BaseType_t xHigherPriorityTaskWoken;
//	xHigherPriorityTaskWoken = pdFALSE;
//	vTaskNotifyGiveFromISR(hrm_task, &xHigherPriorityTaskWoken);
//	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void pah8002_int(void){
	_pah8002_interrupt =1;
}

void pah8002_init(TaskHandle_t  handle) {
//	nrf_gpio_cfg_input(HRM_INT_PIN,NRF_GPIO_PIN_PULLUP);

//	hrm_task=handle;

	nrf_drv_twi_init(&m_twi_pah8002, NULL, twi_event_handler, NULL);
	nrf_drv_twi_enable(&m_twi_pah8002);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    in_config.pull = NRF_GPIO_PIN_PULLUP;

	nrf_drv_gpiote_in_init(HRM_INT_PIN, &in_config, hrm_int_handler);
//	nrf_drv_gpiote_in_event_enable(HRM_INT_PIN, true);

	nrf_gpio_cfg_output(HRM_POWER_PIN);
//	nrf_gpio_cfg_output(HRM_RESET_PIN);
//	nrf_gpio_pin_set(HRM_RESET_PIN);
	nrf_gpio_pin_clear(HRM_POWER_PIN);

//	nrf_gpio_pin_set(HRM_POWER_PIN);
//	vTaskDelay(500);

//	nrf_gpio_pin_set(HRM_RESET_PIN);

//	vTaskDelay(500);

//	vTaskDelay(1500);

	uint8_t ret = 0;
	uint32_t version = 0;
	float myHR = 0;
	int i = 0;

	//Algorithm               initialization
	_pah8002_data.frame_count = 0;
	_pah8002_data.nf_ppg_channel = TOTAL_CHANNELS_FOR_ALG;
	_pah8002_data.nf_ppg_per_channel = HEART_RATE_MODE_SAMPLES_PER_CH_READ;
	_pah8002_data.ppg_data = (int32_t *) pah8002_ppg_data;
#ifdef MEMS_ZERO
	memset(_mems_data, 0, sizeof(_mems_data));
	_pah8002_data.nf_mems = HEART_RATE_MODE_SAMPLES_PER_CH_READ;
	_pah8002_data.mems_data = _mems_data;
#endif

	pah8series_data_t pah8002_data;
//	ret = pah8series_open(pah8002_buffer);
//	version = pah8series_version();
//	pah8002_enter_suspend_mode();
//	init_result = pah8002_enter_normal_mode();

}

void pah8002_shutdown(void){

	_mode=NONE;
	nrf_gpio_pin_clear(HRM_POWER_PIN);

	nrf_drv_twi_disable(&m_twi_pah8002);
	nrf_drv_twi_uninit(&m_twi_pah8002);
	nrf_drv_gpiote_in_uninit(HRM_INT_PIN);
}
