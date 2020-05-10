/* 
* UartObject.cpp
*
* Created: 16.03.2020 20:00:58
* Author: Szymon
*/


#include "UartObject.h"
#include "globals.h"

#include <avr/interrupt.h>

static Buffer receiveBuffer;
bool UARTObject::messagePresent = false;

void UARTObject::Init()
{
	DDRD |= 1 << PIND1;//pin1 of portD as OUTPUT
	DDRD &= ~(1 << PIND0);//pin0 of portD as INPUT
	PORTD |= 1 << PIND0;
	
	//AS described before setting baud rate to 9600
	int UBBRValue = 25;
	//Put the upper part of the baud number here (bits 8 to 11)
	UBRRH = (unsigned char) (UBBRValue >> 8);
	//Put the remaining part of the baud number here
	UBRRL = (unsigned char) UBBRValue;
	//Enable the receiver and transmitter
	//UCSRB = (1 << RXEN) | (1 << TXEN);
		UCSRC = (1<<URSEL)  | (3<<UCSZ0);
	UCSRB = ((1<<RXEN) | (1<<TXEN) | (1<<RXCIE));
	//Set 1 stop bits and data bit length is 8-bit
	//UCSRC = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCSZ2);

}


Buffer UARTObject::GetMessage()
{
	UARTObject::messagePresent = false;
	return receiveBuffer;
}

uint16_t UARTObject::GetValueFromMessage()
{
	uint16_t out;
	out = static_cast<uint16_t>(this->GetMessage()[1]) << 8;
	out |= static_cast<uint16_t>(this->GetMessage()[2]);
	return out;
}

ISR(USART_RXC_vect)
{
	// Called when data received from USART
	static uint8_t counter = 0;
	static uint8_t counting =0;
	// Read UDR register to reset flag
	unsigned char data = UDR;
	
	if(data =='#')
	{
		counting = 1;	
	}

	else if(data == '$' && (counter == (UARTObject::Params::DATA_LENGTH - 1)))
	{
		counting = 0;
		counter = 0;
		UARTObject::messagePresent = true;	
	}
	else if(data == '$')
	{
		counter = 0;
	}
	if(counting)
	{
		receiveBuffer[counter] = data;
		++counter;
	}
}
