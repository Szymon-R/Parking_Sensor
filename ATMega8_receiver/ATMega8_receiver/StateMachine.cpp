/* 
* StateMachine.cpp
*
* Created: 21.03.2020 13:59:45
* Author: Szymon
*/


#include "StateMachine.h"

StateMachine::StateMachine(UARTObject& uart, TimerObject& timer) :
	uart(uart), timer1(timer)
{
	this->currentState = State::IDLE;
	this->nextState = State::IDLE;
}

void StateMachine::SetNextState(State nextState)
{
	this->nextState = nextState;
}
StateMachine& StateMachine::GetInstance()
{
	return StateMachine::machine;
}
void StateMachine::Run()
{
	while(1)
	{
		this->Update();
		switch(this->currentState)
		{
			case State::IDLE:
				uart.Transmit("State: IDLE\n");
				this->SetNextState(State::PROBING);
				_delay_ms(1000);
			break;
			
			case State::PROBING:
				uart.Transmit("State: PROBING\n");
				if(uart.messagePresent)
				{
					uart.Transmit("Probing message\n");
					uint16_t data = uart.GetValueFromMessage();
					this->SetNextState(State::ACTIVE);
					timer1.OverflowCallback = &TransitToDisabling;
					timer1.Run(3000);
				}
				else
				{
					uart.Transmit("No message\n");
					this->SetNextState(State::IDLE);
				}
			break;
			
			case State::ACTIVE:	
			//uart.Transmit("State: ACTIVE\n");
			if(uart.messagePresent)
			{
				uart.Transmit("ResetTimer\n");
				uint16_t data = uart.GetValueFromMessage();
				timer1.Reset();
			}
			break;
			
			case State::DISABLING:
			uart.Transmit("State: DISABLING\n");
			timer1.Stop();
			this->SetNextState(State::IDLE);
			break;
		}
	}	
}

void StateMachine::Update()
{
	this->currentState = this->nextState;
}

void TransitToDisabling()
{
	StateMachine::GetInstance().SetNextState(State::DISABLING);
}