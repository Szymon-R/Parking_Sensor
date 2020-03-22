#include "globals.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "UartObject.h"
#include "TimerObject.h"
#include "StateMachine.h"
//header to enable data flow control over pins

static UARTObject uart2;
static TimerObject timer2;
StateMachine StateMachine::machine(uart2, timer2);



int main(void)
{
	uart2.Init();
	timer2.Init();
	//uart2.Transmit("Start\n");
	sei();
	StateMachine::GetInstance().Run();
	while (1)
	{
		Buffer b1;
		if(UARTObject::messagePresent == true)
		{
			b1 = UARTObject::GetMessage();
			uart2.Transmit("Present\n");
		}
		_delay_ms(1000);
		uart2.Transmit("Alive\n");
	}
}

