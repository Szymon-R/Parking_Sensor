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

enum class TimerPrescaler
{
	VALUE_1 = 1,
	VALUE_8 = 8,
	VALUE_64 = 64,
	VALUE_256 = 256,
	VALUE_1024 = 1024
};

enum class OperationMode
{
	CTC,
	INPUT_CAPTURE
};


class TimerObject
{
public:
	void Init(OperationMode mode = OperationMode::CTC, TimerPrescaler prescaler = TimerPrescaler::VALUE_1);
	TimerObject() = default;
	~TimerObject() =  default;
	static void (*OverflowCallback)();
	void SetTimeoutCounter(uint16_t value);
	void Run();
	void Run(uint16_t timeout);
	void Stop();
	void Reset();
	
private:
	bool overflowed = false;
	TimerPrescaler prescaler;
	OperationMode mode;
	void Timeout(uint16_t interval);
}; //TimerObject

#endif //__TIMEROBJECT_H__
