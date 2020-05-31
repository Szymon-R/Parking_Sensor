#include "globals.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UartObject.h"
#include "TimerObject.h"
#include "StateMachine.h"
#include "I2CDriver.h"
//header to enable data flow control over pins

static UARTObject uart2;
static TimerObject timer2;

StateMachine StateMachine::machine(uart2, timer2);

void func(uint8_t adress)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));
	
	TWDR = adress;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));
	
}

int main(void)
{
	I2CDriver& i2c = I2CDriver::GetInstance();
	uint8_t adress = 8;
	uart2.Init();
	timer2.Init();
	i2c.Init();
	//uart2.Transmit("Start\n");
	sei();
	//StateMachine::GetInstance().Run();
	const char* array="Jakis przykladowy string, ktory z duzym prawdopodobienstwem zostanie wyslany do arduino\n";
	float number = 100;
	while (1)
	{
	/*	Buffer b1;
		if(UARTObject::messagePresent == true)
		{
			b1 = UARTObject::GetMessage();
			uart2.Transmit("Present\n");
		}*/
		++number;
		//i2c.Transmit(adress, &number, sizeof(number));
		_delay_ms(10);
		//uart2.Transmit("Alive\n");
	}
}

