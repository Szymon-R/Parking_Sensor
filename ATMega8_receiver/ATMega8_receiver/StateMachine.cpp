/* 
* StateMachine.cpp
*
* Created: 21.03.2020 13:59:45
* Author: Szymon
*/


#include "StateMachine.h"

StateMachine::StateMachine()
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
					uart.Transmit("Message arrived\n");
					uint16_t data = uart.GetValueFromMessage();
					this->SetNextState(State::ACTIVE);
					//timer1.OverflowCallback = &TransitToDisabling;
					timer1.Run(3000);
				}
				else
				{
					uart.Transmit("No message\n");
					this->SetNextState(State::IDLE);
				}
			break;
			
				case State::ACTIVE:	
				uart.Transmit("State: PROBING\n");
				if(uart.messagePresent)
				{
					uart.Transmit("Message arrived\n");
					uint16_t data = uart.GetValueFromMessage();
				}
			break;
		}
	}	
}

void StateMachine::Update()
{
	this->currentState = this->nextState;
}

void TransitToDisabling(StateMachine& machine)
{
	machine.SetNextState(State::DISABLING);
}