/* 
* TimerObject.h
*
* Created: 21.03.2020 12:36:40
* Author: Szymon
*/


#ifndef __TIMEROBJECT_H__
#define __TIMEROBJECT_H__

#include "globals.h"

#include <avr/io.h>

class TimerObject
{
public:
	void Init();
	TimerObject() = default;
	~TimerObject() =  default;
	static void (*OverflowCallback)();
	void Timeout(uint16_t interval);
	void Run();
	void Run(uint16_t timeout);
	void Stop();
	void Reset();
	
private:
	bool overflowed = false;
}; //TimerObject

#endif //__TIMEROBJECT_H__
