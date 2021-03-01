#ifndef USART_ATMEGA1284_H
#define USART_ATMEGA1284_H

// USART Setup Values
#define F_CPU 1000000UL
//#define F_CPU 64000UL 
#define BAUD_RATE 9000
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)


void initUSART(unsigned char usartNum)
{
	if (usartNum != 1) {

		UBRR0H = (BAUD_PRESCALE >> 8);
		UBRR0L = BAUD_PRESCALE;
		UCSR0B |= (1 << RXEN0)  | (1 << TXEN0);
		UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
		
		
	}
	else {

		UCSR1B |= (1 << RXEN1)  | (1 << TXEN1);
		UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11);
		UBRR1L = BAUD_PRESCALE;
		UBRR1H = (BAUD_PRESCALE >> 8);
	}
}

unsigned char USART_IsSendReady(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << UDRE0)) : (UCSR1A & (1 << UDRE1));
}

unsigned char USART_HasTransmitted(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << TXC0)) : (UCSR1A & (1 << TXC1));
}

unsigned char USART_HasReceived(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << RXC0)) : (UCSR1A & (1 << RXC1));
}

void USART_Send(unsigned char sendMe, unsigned char usartNum)
{
	if (usartNum != 1) {
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = sendMe;
	}
	else {
		while( !(UCSR1A & (1 << UDRE1)) );
		UDR1 = sendMe;
	}
}

unsigned char USART_Receive(unsigned char usartNum)
{
	if (usartNum != 1) {
		while ( !(UCSR0A & (1 << RXC0)) );
		return UDR0; 
	}
	else {
		while ( !(UCSR1A & (1 << RXC1)) );
		return UDR1;
	}
}

#endif USART_ATMEGA1284_H