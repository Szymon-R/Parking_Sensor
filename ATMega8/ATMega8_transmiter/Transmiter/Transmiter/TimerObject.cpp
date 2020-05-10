/* 
* TimerObject.cpp
*
* Created: 21.03.2020 12:36:40
* Author: Szymon
*/


#include "TimerObject.h"
//#include "StateMachine.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void (*TimerObject::OverflowCallback)();

void TimerObject::Init(OperationMode mode, TimerPrescaler prescaler)
{
	this->mode = mode;
	this->prescaler = prescaler;

	//CTC mode
	if (mode == OperationMode::CTC)
	{
		TCCR1B|=(1<<WGM12);
	}

	//Input capture mode
	if (mode == OperationMode::INPUT_CAPTURE)
	{
		TCCR1B|=(1<<WGM12) | (1<<WGM13);
	}

}

void TimerObject::Run()
{
	//enable interrupt
	TIMSK=(1<<OCIE1A);
	
	switch(this->prescaler)
	{
		case TimerPrescaler::VALUE_1:
		TCCR1B |= (1<<CS10);
		break;

		case TimerPrescaler::VALUE_8:
		TCCR1B |= (1<<CS11);
		break;

		case TimerPrescaler::VALUE_64:
		TCCR1B |= (1<<CS10) | (1<<CS11);
		break;

		case TimerPrescaler::VALUE_256:
		TCCR1B |= (1<<CS12);
		break;

		case TimerPrescaler::VALUE_1024:
		TCCR1B|=(1<<CS12) | (1<<CS10);
		break;

		default:
		TCCR1B |= (1<<CS10);
		break;
	}
}

void TimerObject::Run(uint16_t timeout)
{
	this->Timeout(timeout);
	this->Run();
}

void TimerObject::SetTimeoutCounter(uint16_t value)
{
	this->Timeout(value);
}

void TimerObject::Stop()
{
	TCCR1B &= ~((1<<CS12) | (1<<CS12) | (1<<CS10));
	this->Reset();
}

void TimerObject::Timeout(uint16_t interval)
{	
	OCR1A = interval;
}

void TimerObject::Reset()
{
	TCNT1 = 0;
	TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
}

ISR(TIMER1_COMPA_vect)
{
	TimerObject::OverflowCallback();
}