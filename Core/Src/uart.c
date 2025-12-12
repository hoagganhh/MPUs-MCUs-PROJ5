/*
 * uart.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Admin
 */


#include "main.h"
#include "uart.h"
#include "timer.h"

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
char cmd_buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

COMMAND_FSM parser_state = READY;
COMMAND_STATE cmd_state = OKAY;

extern UART_HandleTypeDef huart2;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
        if(temp != ' ' && temp != '\n'){ // ignore spaces and enters
            buffer[index_buffer] = temp;
            index_buffer++;
            if(index_buffer >= MAX_BUFFER_SIZE){
                index_buffer = 0;
            }
            buffer_flag = 1;
        }
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}

void receive_enable_wrapper(void){
	HAL_UART_Receive_IT(&huart2, &temp, 1);
}

char transmit_buffer[64] = {1};
void command_parser_fsm(){
	switch(parser_state){
	case READY: //initialize fsm parsing

		if((char)temp == '!'){ //if detect !, switch to begin state
			parser_state = BEGIN;
			memset(cmd_buffer, '\0', sizeof(cmd_buffer));
		}

		break;
	case BEGIN:

		if((char)temp == '#'){ //if detect end command
            int end_index = index_buffer - 2; //index of last char before '#'
            if (end_index < 0) {
                end_index += MAX_BUFFER_SIZE; //wrap around
            }
            int start_index = -1; //invalid index until found
            int length = 0;


			for(int i = 0; i < MAX_BUFFER_SIZE; i++){ //search back for ! for the whole array
				int current_index = end_index - i;
				if(current_index < 0){ //wrapped around
					current_index += MAX_BUFFER_SIZE;
				}
				if(buffer[current_index] == '!'){ //if there is, buffer has not overflowed yet
					start_index = current_index + 1;
					if(start_index >= MAX_BUFFER_SIZE){
						start_index = 0;
					}
					break; //stop loop
				}
				length++;
			}

            if (start_index != -1 && length < MAX_BUFFER_SIZE) {
                for(int i = 0; i < length; i++){ //copy character from start index to end index
                    int current_index = start_index + i;
                    if(current_index >= MAX_BUFFER_SIZE){
                    	current_index -= MAX_BUFFER_SIZE;
                    }
                    cmd_buffer[i] = buffer[current_index];
                }
                cmd_buffer[length] = '\0'; // null terminate
                parser_state = GET_COMMAND;
            } else {
                parser_state = READY; // failed to find '!', reset
            }
		}
		break;
	case GET_COMMAND:

		if(strcmp(cmd_buffer, "RST") == 0){ //check if command buffer is reset, switch command state to reset
			cmd_state = RESET;
			sprintf(transmit_buffer, "State: RST\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t *)transmit_buffer, strlen(transmit_buffer), HAL_MAX_DELAY);
		}
		else if(strcmp(cmd_buffer, "OK") == 0){ //check if command buffer is ok, switch command state to ok
			cmd_state = OKAY;
			sprintf(transmit_buffer, "State: OK\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t *)transmit_buffer, strlen(transmit_buffer), HAL_MAX_DELAY);
		}

		parser_state = READY;
		break;
	default:
		parser_state = READY;
		break;
	}
}

uint8_t value_read = 0;
uint32_t ADC_value = 0;
extern ADC_HandleTypeDef hadc1;
void uart_communication_fsm(void){
	switch(cmd_state){
	case OKAY:
		three_sec_init();
		value_read = 0;
		break;
	case RST:
		if(three_sec_flag == 1){
			three_sec_flag = 0;
			if(value_read == 0){
				value_read = 1;
				ADC_value = HAL_ADC_GetValue(&hadc1);
			}
			sprintf(transmit_buffer, "!ADC=%ld#\r\n", ADC_value);
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)transmit_buffer, strlen(transmit_buffer));
		}
		break;
	default:
		break;
	}
}
