/*
 * Transciver Custom.c
 *
 * Created: 1/12/2021 3:22:26 PM
 * Author : oscar
 */ 

#include "..//..//scheduler.h"
#include "..//..//CONTROL.H"
#include <avr/io.h>

#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define Idata (((PINA & 0x02)) == 0x02)
#define Gdata (PINA)
#define Odata (PORTD)
#define LEDOUT PORTB
#define ID (0x0002) // starts at 0x01. Cannot be 0.

enum L_States { Idle, Check, Store, CheckTask, StartSend, Send, EndSend, RespondID, RespondTemp, Gtemp};
	
const int InstructLength = 10; 
	
int Tstate = 0;
//unsigned short temp = 0x0002;
//unsigned long Total = 0;
short data = 0x0000;
short TempTem;


void Set_Clock(int tic);
void delay_ms(int miliSec); //for 8 Mhz crystal
int read_ADC(char pin);

	
int TickFct_Leader(int state) {
	
	
	
	switch(state) {
		case -1:
			state = Idle;
			Odata = 0x0000;
		break;
		
		case Idle:
		
			Tstate = 0;
			LEDOUT = 0x00;
			
			///*
			if(Idata)
			{
				if(Timer(1))
				{
					state = Check;
				}
				
			}//*/
			
			/*
			if(Timer(100))
			{
				data = 0x0135;
				state = StartSend;
			}//*/
			
			
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
					
					state = CheckTask;
					Tstate = 0;
					
				}
				else
				{
					data = 0x0000;
					state = Idle;
					Tstate = 0;
				}
			}
		break;
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		case CheckTask:
		
			Tstate = 0;
			if(data == 0x00)
			{
				state = Idle;
			}
			else if(true)//Timer(5))
			{
				//	00  -  temp response
				//	01  -  temp request
				//	10  -  sync request
				//	11  -  sync response
				
				//if((((data & 0x0003) > 0x0002)) && (data > 0x00FF) && ((data) < 0x0200))
				
				if( ((data & 0x0300) == 0x0100) && ((data & 0x000f) == ID))
				{
					//state = Gtemp;
					
					//state = StartSend;
					
					
					LEDOUT = data;
					
					if(Timer(300))
					{
						//state = Idle;
						state = Gtemp;
						//state = StartSend;
					}
				}
				else
				{
					state = Idle;
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
		
		
		
		case Gtemp:
			TempTem = read_ADC(0);
			data = ((0x00FF) & TempTem);
			//data = 0x0045;
			Tstate = 0;
			state = StartSend;
		
		break;
		
		
	}


	

	return state;
}


int main(void)
{
	DDRB = 0xFF; LEDOUT = 0x00; // ID LED's
	DDRD = 0xFF; PORTD = 0x00; // RF Input and Output
	DDRA = 0x00; PORTA = 0xFF; // Temp Resistor
	
	//unsigned short x =ADC_read(1);
	
	LEDOUT = ID;
	
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




int read_ADC(char pin)
{
	ADCSRA = (1<<ADEN)|(1<< ADPS1)|(1<<ADPS2);
	ADMUX = (3 << REFS0)|(1<<ADLAR)|(pin << MUX0);
	_delay_us(10);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCH;
}