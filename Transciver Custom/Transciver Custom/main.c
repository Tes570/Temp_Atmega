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
		
			if(data == 0x00)
			{
				state = Idle;
			}
			else
			{
				//	00  -  temp request
				//	01  -  temp response
				//	10  -  sync request
				//	11  -  sync response
				
				if(((data & 0xC0) == 0x00) && ((data & 0x03) == ID) )// 00
				{
					data = (0x40 | ID);
					state = StartSend;
				}
				else if((data & 0xC0) == 0x80) // 10
				{
					data = (0xC0 | ID);
					state = StartSend;
				}
			}
			
			
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
	DDRB = 0xFF; PORTB = 0x00; // ID LED's
	DDRD = 0xFE; PORTD = 0x01; // RF Input and Output
	DDRA = 0x00; PORTA = 0xFF; // Temp Resistor
	
	PORTB = ID;
	
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

