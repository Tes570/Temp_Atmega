/*
 * Temp_sensor.c
 *
 * Created: 1/16/2021 9:42:22 PM
 * Author : ME
 */ 
#ifndef F_CPU
#define F_CPU 1600000UL
#endif
#include <avr/io.h>

#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//#include "io_2.h"

void delay_ms(int miliSec) //for 8 Mhz crystal
{
	int i,j;
	for(i=0;i<miliSec;i++)
	for(j=0;j<775;j++)
	{
		asm("nop");
	}
}

int main(void)
{
	 DDRC=0xFF; PORTC=0x00;
	 DDRD= 0xFF; PORTD= 0x00;
	 DDRA=0x00; PORTA = 0xFF;
	 DDRB = 0xFF; PORTB = 0x00;//output led 

	 uint16_t adc_result0;
	 int Volts;
	 char buffer[10];
	 int tempatureC;
	 int binary[8];
	 int remainder;
	 int i=0;
	 int j=0;
	 char orValue= 0x00;
	 int tempF;
	 int temp;
	 setupADC();
	// LCD_init();
	 ADC_read();
	 //LCD_DisplayString(1,"Hello World");

		while (1)
		{
			//adc_result0 = ADC_read(0);      // read adc value at PA0
			unsigned short x =ADC_read(1);

			unsigned char lower8 = (char)x;
			PORTC = lower8;
			unsigned char upper2 = (char)(x >> 8);
			PORTD = upper2;
			//temp= ((x*4.88)-500);
			//temp=(temp/10);
			//PORTC=temp;
			delay_ms(2000);
			/*
			Volts= (x)*(5/1024);
			tempatureC=(Volts-.5)*100;
			/*
			tempF=(tempatureC*(9/5)+32);
			remainder=decimalToBinary(127);
			PORTC=remainder;
			/*while(remainder>0){
				binary[i]=remainder%2;
				remainder=remainder/2;
				++i;
			}*/
			//PORTC= orValue|binary;
	}

}
void setupADC()
{
	ADMUX=(1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}
int ADC_read(int ch)
{
	// select the corresponding channel 0~7
	//int ADC_result=0;
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;

	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);

	// wait for conversion to complete
	// ADSC becomes '0' again
	
	while(ADCSRA & (1<<ADSC));
	//ADC_result=ADCL;
	//ADC_result |=(ADC<<8);
	return (ADC);
}