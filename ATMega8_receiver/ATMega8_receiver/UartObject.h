/* 
* UartObject.h
*
* Created: 16.03.2020 20:00:58
* Author: Szymon
*/


#ifndef __UARTOBJECT_H__
#define __UARTOBJECT_H__

#include <avr/io.h>
#include <limits.h>

#include "Buffer.h"

class UARTObject
{
	public:
		struct Params
		{
			static constexpr int BAUD = 9600;
			static constexpr int DATA_LENGTH = 4;
		};
		void Init();
		template<typename T>
		void Transmit(T* dataPtr, uint8_t size = UCHAR_MAX);
		void ReceiverRun();
		uint16_t GetValueFromMessage();
		static Buffer GetMessage();
		static bool messagePresent;
	private:
		Buffer message;
			
};

template<typename T>
void UARTObject::Transmit(T* dataPtr, uint8_t size)
{
	const uint8_t* iter = reinterpret_cast<const uint8_t*>(dataPtr);
	const uint8_t* ending = iter + size*sizeof(uint8_t);
	while(*iter != '\0' && iter<ending)
	{
		while (! (UCSRA & (1 << UDRE)) );
		{
			//once transmitter is ready sent eight bit data
			UDR = *iter;
			++iter;
		}
	}
}
#endif //__UARTOBJECT_H__
