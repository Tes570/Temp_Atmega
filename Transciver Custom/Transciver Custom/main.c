/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"
#include "..//..//CONTROL.H"
#include <avr/io.h>

#define Idata (((PINA & 0x01)) == 0x01)
#define Gdata (PINA)
#define Odata (PORTD)
#define LEDOUT PORTB

enum L_States { Idle, Check, Store, CheckTask, StartSend, Send, EndSend, RespondID, RespondTemp, Demo, Prin};
	
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
			Odata = 0x00;
		break;
		
		case Idle:
		
			//data = 0x02;
			//data = 0x50;
			//data = 0x12;
			//data = 'A';
			//data = 'z';
			Tstate = 0;
			LEDOUT = 0x00;
			
			//LEDOUT = data;
			
			
			///*
			if(Idata)
			{
				if(Timer(1))
				{
					state = Check;
				}
				
			}//*/
			
			/*
			if(Timer(4))
			{
				data = 0x65	;
				state = StartSend;
			} //*/
		break;
		
		case Check:
			
			if(Timer(4))
			{
				if(Idata)
				{
					data = 0x00;
					state = Store;
					Tstate = 0;
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
					if(Idata)
					{
						data = ((0x01 << (Tstate))) | data;
					}
					
					++Tstate;
					
				}
				else if(Idata)
				{
					
					state = Prin;
					LEDOUT = data;
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
			
			Odata = 0x01;
			if(Timer(4))
			{
				if(Tstate < 1)
				{
					++Tstate;
				}
				else
				{
					state = Send;
					Tstate = 0;
					//Odata = 0x00;
					
					if(((data) & 0x01) == 0x01)
					{
						Odata = 0x01;
					}
					else
					{
						Odata = 0x00;
					}
				}
			}
		break;
		
		case Send:
			
			//Odata = 0x00;
			
			if(Timer(4))
			{
				
				if(Tstate < InstructLength)
				{
					++Tstate;
					//Odata = (data >> (Tstate)) & 0x02;
					if(((data >> (Tstate)) & 0x01) == 0x01)
					{
						Odata = 0x01;
					}
					else
					{
						Odata = 0x00;
					}
					
					
				}
				
			}
			
			if(Tstate >= InstructLength)
			{
				
				state = EndSend;
			}
		break;
		
		case EndSend:
			Odata = 0x01;
			if(Timer(4))
			{
				state = Demo;
				Odata = 0x00;
			}
		break;
		
		case Demo:
			if(Timer(200))
			{
				
				state = Idle;
			}
		break;
		
		case Prin:
			//LEDOUT = data;
			if(Timer(800))
			{
				state = Idle;
			}
		break;
		
		
	}


	

	return state;
}


int main(void)
{
	DDRB = 0xFF; LEDOUT = 0x00; // ID LED's
	DDRD = 0xFF; PORTD = 0x00; // RF Input and Output
	DDRA = 0x00; PORTA = 0xFF; // Temp Resistor
	
	//DDRC = 0xFF; PORTC = 0x00; // ID LED's
	
	//LEDOUT = ID;
	
	//Set_Clock(500);
	tasksNum = 1;
	task tsks[1];
	tasks = tsks;
	
	// define tasks
	unsigned char i=0;
	tasks[i].state = Idle;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Leader;
	
	TimerSet(tasks[i].period);
	TimerOn();
	
    while (1) 
    {
		
    }
}

