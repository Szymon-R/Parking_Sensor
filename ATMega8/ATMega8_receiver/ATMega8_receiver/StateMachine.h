/* 
* StateMachine.h
*
* Created: 21.03.2020 13:59:45
* Author: Szymon
*/


#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "globals.h"
#include "UartObject.h"
#include "TimerObject.h"

#include <util/delay.h>

enum class State
{
	IDLE,
	PROBING,
	ACTIVE,
	DISABLING		
};

class StateMachine
{
public:
	StateMachine(UARTObject& uart, TimerObject& timer);
	void SetNextState(State nextState);
	void Run();
	void Update();
	static StateMachine& GetInstance();
	
private:
	static StateMachine machine;	
	UARTObject& uart;
	TimerObject& timer1;
	State currentState;
	State nextState;
};

void TransitToDisabling();

#endif //__STATEMACHINE_H__
