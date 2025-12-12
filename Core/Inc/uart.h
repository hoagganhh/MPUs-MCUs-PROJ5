/*
 * uart.h
 *
 *  Created on: Nov 13, 2025
 *      Author: Admin
 */

#ifndef INC_UART_H_
#define INC_UART_H_

typedef enum{
	READY,
	BEGIN,
	GET_COMMAND
}COMMAND_FSM;

typedef enum{
	RST,
	OKAY
}COMMAND_STATE;

extern COMMAND_FSM parser_state;
extern COMMAND_STATE cmd_state;
extern char command_buffer[];
extern uint8_t buffer_flag;

void command_parser_fsm(void);
void uart_communication_fsm(void);
void receive_enable_wrapper(void);

#endif /* INC_UART_H_ */
