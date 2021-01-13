/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"

#include <avr/io.h>

unsigned char temp = 0x02;
enum L_States { Leader_Output, Hold, Stop};
	
int Tstate;
unsigned long Total = 0;

void Set_Clock(int tic);
	
int TickFct_Leader(int state) {
	switch(state) {
		case -1:
			state = Leader_Output;
		break;
		
		case Leader_Output:
			if(Total >= 50)
			{
				state = Hold;
			}
		break;
		
		case Hold:
			if(Total >= 1000)
			{
				state = Stop;
			}
			
		break;
		
		case Stop:
			state = -1;
		break;
	}

	switch(state) {
		case -1:
			//Set_Clock(500);
			temp = 0x02;
		break;
		
		case Leader_Output:
		
			temp ^= 0x02;
			PORTD = temp;
			PORTA = temp;
			Total += 1;
		
		break;
		
		case Hold:
			Total += 1;
			temp = 0x00;
			PORTD = temp;
			PORTA = temp;
		break;
		
		case Stop:
			Total = 0;
			temp = 0x02;
			PORTD = temp;
			PORTA = temp;
			//Set_Clock(100);
		break;
		
	}

	return state;
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	
	DDRD = 0xFE; PORTD = 0x00;
	
	//Set_Clock(500);
	tasksNum = 1;
	task tsks[1];
	tasks = tsks;
	
	// define tasks
	unsigned char i=0;
	tasks[i].state = -1;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Leader;
	
	TimerSet(tasks[i].period);
	TimerOn();
	
    /* Replace with your application code */
    while (1) 
    {
		//Tstate = TickFct_Leader(Tstate);
    }
}

