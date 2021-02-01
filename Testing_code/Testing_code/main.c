/*
 * Testing_code.c
 *
 * Created: 1/12/2021 11:04:15 AM
 * Author : Bhrayan Escobar
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
 
int read_ADC(char pin)
{
	ADCSRA = (1<<ADEN)|(1<< ADPS1)|(1<<ADPS2);
	ADMUX = (3 << REFS0)|(1<<ADLAR)|(pin << MUX0); 
	_delay_us(10);        
	ADCSRA |= (1<<ADSC);       
	while (ADCSRA & (1<<ADSC)); 
	return ADCH;                
}
int main(void)
{
	DDRC=0xff;
	DDRD=0xff;

while (1)
{
 PORTC = read_ADC(0);     
}
	return 0;
}			
