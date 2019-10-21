/*
 * app_hrm.c
 *
 *  Created on: Sep 26, 2019
 *      Author: Andrzej Surowiec
 */

#include "app_hrm.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "icon.h"
#include "nrf_gfx.h"
#include "pah8002.h"
#include "vibration.h"

static const nrf_lcd_t * p_lcd = &nrf_lcd_lpm013m126a;
static const nrf_gfx_font_desc_t * p_font = &orkney_8ptFontInfo;

int sample_counter=0;
uint32_t frame_counter=0;
int sampling_started=0;
uint32_t graph_start=HRM_GRAPH_WIDTH;
int available_samples=0;
int graph_pointer=0;
int first_sample=0;

arm_rfft_instance_q31 fft_instance;
q31_t fft_buffer[FFT_LENGTH*2];
q31_t output[FFT_LENGTH*2];

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 20 Hz

fixed point precision: 24 bits

* 0 Hz - 3 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 5 Hz - 10 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define FILTER_TAP_NUM 13

static int filter_taps[FILTER_TAP_NUM] = {
  -96636,
  -440161,
  -667934,
  -289846,
  942690,
  2422114,
  3093873,
  2422114,
  942690,
  -289846,
  -667934,
  -440161,
  -96636
};




static q31_t fir_state[SAMPLES_PER_SET + FILTER_TAP_NUM - 1];
uint32_t blockSize = SAMPLES_PER_SET;
uint32_t numBlocks = HRM_GRAPH_WIDTH/SAMPLES_PER_SET;
arm_fir_instance_q31 fir = {FILTER_TAP_NUM, fir_state, filter_taps};

#define DC_BLOCK_PARAM 700/1000

float hr=60.0;
static char hr_str[32];
float avgi=1;
float w=0.1;
int frames_per_beat;

void graph_shift_in(int step);

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void draw_hrm(void) {

	if (available_samples>20){
		graph_shift_in(2);
	}

	nrf_gfx_point_t point = NRF_GFX_POINT(5,150);
	nrf_gfx_point_t last_point= NRF_GFX_POINT(5,150);

	int max;
	int min;

	max=0;
	min=hrm_graph[0];

	volatile int maxf;
	volatile int minf;

	maxf=0;
	minf=output[0];

	for (int i = 0; i < HRM_GRAPH_WIDTH; i++){
		max=MAX(max,hrm_graph[i]);
		min=MIN(min,hrm_graph[i]);

		maxf=MAX(maxf,output[i]);
		minf=MIN(minf,output[i]);
	}

	point.x=5;

	nrf_gfx_point_t cur = NRF_GFX_POINT(5+HRM_GRAPH_WIDTH-((graph_start)%HRM_GRAPH_WIDTH),40);
	nrf_gfx_point_draw(p_lcd, &cur, CYAN);

	for (int i = 0; i < HRM_GRAPH_WIDTH; i++) {

		point.x=5+i;
		volatile int y = map(hrm_graph[i],min,max,0,100);
		point.y=150-y;
//		nrf_gfx_point_draw(p_lcd, &point, GREEN);
		nrf_gfx_line_t line= NRF_GFX_LINE(last_point.x,last_point.y, point.x,point.y,1);
		nrf_gfx_line_draw(p_lcd, &line, GREEN);
		last_point=point;

//		y = map(output[i],minf,maxf,0,100);
//		point.y=150-y;
//		nrf_gfx_line_t linef = NRF_GFX_LINE(point.x,150-y, point.x,150,1);
//		nrf_gfx_line_draw(p_lcd, &linef, BLUE);

//		y = map(hrm_values[2][i],min[2],max[2],0,100);
//		point.y=150-y;
//		nrf_gfx_point_draw(p_lcd, &point, BLUE);

//		point.x++;

	}

	if (frame_counter%frames_per_beat<2){
		lcd_draw_icon(8,160, icons[ICON_HEART_SMALL]);
	}
	else{
		lcd_draw_icon(8,160, icons[ICON_HEART_BIG]);
	}

	nrf_gfx_point_t text_start = NRF_GFX_POINT(24, 160);
    snprintf(hr_str,sizeof(hr_str),"HR: %.2f",hr);
    nrf_gfx_print(p_lcd, &text_start, WHITE, hr_str, p_font, true);

	nrf_gfx_display(p_lcd);

//	if (graph_start>=HRM_GRAPH_WIDTH){
//		graph_start=0;
//	}
	frame_counter++;
}



void graph_shift_in(int step){

	for (int i = 0; i < HRM_GRAPH_WIDTH-step; i++) {
		hrm_graph[i]=hrm_graph[i+step];
	}

	for (int i = 0; i < step; i++) {
		int x=HRM_GRAPH_WIDTH-step+i;

		// DC Blocker filter
		hrm_graph[x]=hrm_values[graph_pointer]-hrm_values[mod(graph_pointer-1,HRM_BUFFER_WIDTH)]+hrm_graph[x-1]*DC_BLOCK_PARAM;
//		hrm_graph[x]=hrm_values[graph_pointer];

		graph_pointer++;
		if (graph_pointer==HRM_BUFFER_WIDTH){
			graph_pointer=0;
		}
		available_samples--;
	}

//	  arm_fir_init_q31(&fir, FILTER_TAP_NUM, filter_taps, fir_state, blockSize);
//	  for(int i=0; i < numBlocks; i++)
//	  {
//	    arm_fir_q31(&fir, hrm_graph + (i * blockSize), hrm_graph_filtered + (i * blockSize), blockSize);
//	  }



	arm_rfft_init_q31(&fft_instance, FFT_LENGTH/*bin count*/, 0/*forward FFT*/, 1/*output bit order is normal*/);

	memcpy(fft_buffer,hrm_graph,FFT_LENGTH*4);
	arm_rfft_q31(&fft_instance, fft_buffer, output);
	arm_abs_q31(output,output,FFT_LENGTH*2);


	int max=0;
	int maxi=0;

	for (int i = 7; i < 40; i++) {
		if(output[i]>max){
			max=output[i];
			maxi=i;
		}
	}

	avgi=w*maxi + (1-w)*avgi;

	hr=avgi*4.6875;

	frames_per_beat=600/hr;
}

void hrm_add_samples(int32_t *samples){

	q31_t sample_buffer[SAMPLES_PER_SET];

	for (int i = 0; i < SAMPLES_PER_SET; i++){

		int current_sample=samples[i*4+2];
//		hrm_values[i+sample_counter*SAMPLES_PER_SET]=current_sample;
		sample_buffer[i]=current_sample;
		if (first_sample==0&&current_sample!=0){
			first_sample=current_sample;
		}

	}

//	arm_fir_q31(&fir, sample_buffer, (q31_t *)hrm_values + (sample_counter*SAMPLES_PER_SET), blockSize);
	memcpy(hrm_values + (sample_counter*SAMPLES_PER_SET),sample_buffer,SAMPLES_PER_SET*4);

	available_samples+=SAMPLES_PER_SET;
	sample_counter++;
	if (sample_counter>=(HRM_BUFFER_WIDTH/SAMPLES_PER_SET)){
		sample_counter=0;
	}
}

void hrm_handle_button_evt(button_event_t *evt){



}
