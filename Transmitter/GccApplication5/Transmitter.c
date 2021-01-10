/*
 * GccApplication5.c
 *
 * Created: 1/8/2021 11:46:30 PM
 * Author : ME
 */ 
#include <avr/io.h>


#define F_CPU 10000000UL  
#include "SCHEDULER.h"
#include "USART_ATMEGA1284.h"

unsigned char temp = 0x01;
enum L_States { Leader_Output };
int TickFct_Leader(int state) {
	switch(state) { 
		case -1: 
		state = Leader_Output;
		break;
		
		case Leader_Output:
		break;
		
		default:
		state = -1;
		break;
	}

	switch(state) { 
		case -1: 
		state = Leader_Output;
		break;
		
		case Leader_Output:
		if (USART_IsSendReady(0))
		{
			temp ^= 0x03;
			PORTA = temp;
			USART_Send(0X55 , 0);
		}
		break;
		
		default:
		state = -1;
		break;
	}

	return state;
}




int main(void) {

	
	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	initUSART(0);
	
	tasksNum = 1; 
	task tsks[1]; 
	tasks = tsks; 
	
	// define tasks
	unsigned char i=0; 
	tasks[i].state = -1;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Leader;
	
	TimerSet(1000); 
	TimerOn();

	while(1) {} 
	
	
	return 0;
}

