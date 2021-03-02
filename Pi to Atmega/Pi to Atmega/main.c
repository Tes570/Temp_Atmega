/*
 * Pi to Atmega.c
 *
 * Created: 3/1/2021 8:37:52 PM
 * Author : oscar
 */ 

#include <avr/io.h>
#include "..//..//USART.h"

int main(void)
{
	
	initUSART(0);
	DDRA = 0xFF; PORTA = 0x00; // ID LED's
	
    /* Replace with your application code */
    while (1) 
    {
		///*
		if (USART_IsSendReady(0))
		{
			USART_Send((0x02));
		}//*/
		
		//PORTA = USART_Receive();
		
    }
}

