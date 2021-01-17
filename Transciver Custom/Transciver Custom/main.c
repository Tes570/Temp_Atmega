/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"
#include "..//..//CONTROL.H"
#include <avr/io.h>

#define Idata ((PORTD & 0x01) == 0x01)
#define Gdata (PORTD)
#define Odata (PORTD)

enum L_States { Idle, Check, Store, EndGet, CheckTask, StartSend, Send, EndSend, RespondID, RespondTemp};

const int InstructLength = 8;
const char ID = 0x01; // starts at 0x01. Cannot be 0.
	
int Tstate = 0;
unsigned char temp = 0x02;
unsigned long Total = 0;
char data = 0x00;


void Set_Clock(int tic);
	
int TickFct_Leader(int state) {
	switch(state) {
		case -1:
			state = Idle;
		break;
		
		case Idle:
		
			data = 0x00;
			Tstate = 0;
			//PORTA = 0x03;
			
			
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
					data = 0x00;
				}
				else
				{
					state = Idle;
				}
			}
		break;
		
		case Store:
			//PORTA = 0x03;
			if(Timer(4))
			{
				if(Tstate < InstructLength)
				{
					data = (Gdata << Tstate) | data;
					++Tstate;
					
				}
				else
				{
					state = EndGet;
				}
			}
		break;
		
		case EndGet:
			if(Timer(4))
			{
				if(Idata)
				{
					state = CheckTask;
				}
				else
				{
					state = Idle;
				}
			}
			
		break;
		
		case CheckTask:
			state = Idle;
		break;
		
		case StartSend:
			if(Timer(4))
			{
				if(Tstate < 2)
				{
					Odata = 0x02;
					++Tstate;
				}
				else
				{
					state = Send;
				}
			}
		break;
		
		case Send:
			if(Timer(4))
			{
				if(Tstate < InstructLength)
				{
					Odata = (data << (Tstate + 1));
					++Tstate;
				}
				else
				{
					state = EndSend;
				}
			}
		break;
		
		case EndSend:
			Odata = 0x02;
			if(Timer(4))
			{
				state = Idle;
			}
		break;
		
		
	}


	

	return state;
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	
	DDRD = 0xFE; PORTD = 0x01;
	
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

