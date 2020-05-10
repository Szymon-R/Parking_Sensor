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

void TimerObject::Init()
{
	//CTC mode
	TCCR1B|=(1<<WGM12);
//	this->Run(3000);
}

void TimerObject::Run()
{
	//enable interrupt
	TIMSK=(1<<OCIE1A);
	
	//prescaler 1024
	TCCR1B|=(1<<CS12) | (1<<CS10);	
}

void TimerObject::Run(uint16_t timeout)
{
	this->Timeout(timeout);
	this->Run();
}

void TimerObject::Stop()
{
	//prescaler 1024
	TCCR1B = 0;
	this->Reset();
}

void TimerObject::Timeout(uint16_t interval)
{	
	OCR1A = interval;
}

void TimerObject::Reset()
{
	TCNT1 = 0;
}

ISR(TIMER1_COMPA_vect)
{
	TimerObject::OverflowCallback();
}