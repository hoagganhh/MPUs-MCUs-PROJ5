/*
 * timer.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Admin
 */


#include "main.h"
#include "timer.h"

uint16_t counter1 = 0;
uint16_t counter3 = 0;
uint8_t three_sec_flag = 0;
uint8_t one_sec_flag = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(counter1 >= 50){
		    one_sec_flag = 1;
		    counter1 = 0;
		}

		if(counter3 >= 150){
			three_sec_flag = 1;
			counter3 = 0;
		}
		counter1++;
		counter3++;
	}
}

void three_sec_init(void){
	three_sec_flag = 1;
	counter3 = 0;
}
