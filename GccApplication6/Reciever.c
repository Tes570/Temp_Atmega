/*
 * GccApplication6.c
 *
 * Created: 1/9/2021 5:39:43 PM
 * Author : ME
 */ 

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "scheduler.h"


enum States { Wait, Output };
int TickFct_Follower(int state) {
	switch(state) { 
		case -1: 
		state = Wait;
		break;
		
		case Wait:
		if(USART_HasReceived(0)){
			state = Output;
		}
		break;
		
		case Output:
		state = Wait;
		break;
		
		default:
		state = -1;
		break;
	}

	switch(state) {
		case -1:
		state = Wait;
		break;
		
		case Wait:
		break;
		
		case Output:
		PORTA = USART_Receive(0);
		break;
		
		default:
		state = -1;
		break;
	}

	return state;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	
	initUSART(0);
	
	tasksNum = 1; 
	task tsks[1]; 
	tasks = tsks; 
	
	
	unsigned char i=0; 
	tasks[i].state = -1;
	tasks[i].period = 50;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Follower;
	
	TimerSet(50); 
	TimerOn();

	while(1) {} 
	
	
	return 0;
}
