/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"
#include "..//..//CONTROL.H"
#include <avr/io.h>

enum L_States { Idle, Check, Store, EndGet, CheckTask, StartSend, Send, EndSend, RespondID, RespondTemp};

const int InstructLength = 8;
	
int Tstate = 0;
unsigned char temp = 0x02;
unsigned long Total = 0;

#define Idata ((PORTD & 0x01) == 0x01)
#define Odata (PORTD & 0x02)


void Set_Clock(int tic);
	
int TickFct_Leader(int state) {
	switch(state) {
		case -1:
			state = Idle;
		break;
		
		case Idle:
			if(Idata)
			{
				if(Timer(1))
				{
					state = Check;
				}
			}
		break;
		
		case Check:
			if(Timer(4))
			{
				if(Idata)
				{
					state = Store;
				}
				else
				{
					state = Idle;
				}
			}
		break;
		
		case Store:
			if(Timer(4))
			{
				if(Tstate < InstructLength)
				{
					++Tstate;
					
				}
				else
				{
					
				}
			}
		break;
		
	}

	switch(state) {
		case -1:
			//Set_Clock(500);
			temp = 0x02;
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
	
    while (1) 
    {
		
    }
}

