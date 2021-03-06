/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"
//#include "..//..//usart_ATmega1284.h"
#include "..//..//CONTROL.H"
#include <avr/io.h>

#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define Idata ((((PINA & 0x02)) == 0x02) || (((PINA & 0x08)) == 0x08))
#define Jdata (((PINA & 0x01)) == 0x01)
#define Tdata (((PINA & 0x04)) == 0x04)
#define Gdata (PINA)
#define Odata (PORTD)
#define BOUT PORTB
#define COUT PORTC
#define ID (0x0003) // starts at 0x01. Cannot be 0.

enum L_States { Idle, Check, Store, CheckTask, StartSend, Send, EndSend, RespondID, RespondTemp};
	
const int InstructLength = 10; 
	
int Tstate = 0;
//unsigned short temp = 0x0002;
//unsigned long Total = 0;
short data = 0x0000;
short TempTem;


void Set_Clock(int tic);


	
int TickFct_Leader(int state) {
	
	//((COUT) = (Gdata & 0x04));
	/*
	if(!Tdata)
	{
		state = -1;
	}//*/
	
	switch(state) {
		case -1:
			state = Idle;
			Odata = 0x0000;
			BOUT = 0x00;
			COUT = 0x00;
		break;
		
		case Idle:
		
			Tstate = 0;
			BOUT = 0x00;
			COUT = 0x00;
			
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
					data = 0x0000;
					state = Store;
					Tstate = 0;
				}
				else
				{
					data = 0x0000;
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
						data = ((0x0001 << (Tstate))) | data;
					}
					
					++Tstate;
					
				}
				else if(Idata)
				{
					if(Tdata)
					{
						state = StartSend;
						Tstate = 0;
					}
					else
					{
						state = CheckTask;
						Tstate = 0;
					}
					
					
				}
				else
				{
					data = 0x0000;
					state = Idle;
					Tstate = 0;
				}
			}
		break;
		
		
		case StartSend:
		
			Odata = 0x0001;
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
				
					if(((data) & 0x0001) == 0x0001)
					{
						Odata = 0x0001;
					}
					else
					{
						Odata = 0x0000;
					}
				}
			}
		break;
		
		case Send:
		
			if(Timer(4))
			{
			
				if(Tstate < InstructLength)
				{
					++Tstate;
					if(((data >> (Tstate)) & 0x0001) == 0x0001)
					{
						Odata = 0x0001;
					}
					else
					{
						Odata = 0x0000;
					}
				
				
				}
			
			}
		
			if(Tstate >= InstructLength)
			{
				Tstate = 0;
				state = EndSend;
			}
		break;
		
		case EndSend:
			Odata = 0x0001;
			if(Timer(4))
			{
				state = Idle;
				//state = -1;
				Odata = 0x0000;
			}
		break;
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		case CheckTask:
		
			Tstate = 0;
			///*
			if((data & 0x0300) == 0x0200)
			{
				BOUT = data;
				COUT = 0x03;
				
				_delay_us(10000000); //10 Sec
				//_delay_us(10000000); //10 Sec
				//_delay_us(10000000); //10 Sec
				//_delay_us(10000000); //10 Sec
				//_delay_us(2000000); // 2 Sec
				state = Idle;
				BOUT = 0x00;
				COUT = 0x00;
				_delay_us(10000);
				
			}
			else
			{
				state = Idle;
			}//*/
			
			
			//COUT = ((data >> 8) | 0x04);
			//_delay_us(1000000);
			//state = Idle;
			
		break;
		
		
			
	}

	
	

	return state;
}


int main(void)
{
	DDRB = 0xFF; BOUT = 0x00; // ID LED's
	DDRC = 0xFF; COUT = 0x00; // ID LED's
	
	DDRD = 0xFF; PORTD = 0x00; // RF Input and Output
	DDRA = 0x00; PORTA = 0xFF; // Temp Resistor
	
	//unsigned short x =ADC_read(1);
	
	
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


