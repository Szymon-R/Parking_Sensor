/* 
* Buffer.h
*
* Created: 18.03.2020 14:44:31
* Author: Szymon
*/


#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <avr/io.h>
#include <limits.h>

class Buffer
{
	public:
		uint8_t GetSize()
		{
			return this->COMMAND_LENGTH;
		}
		uint8_t& operator[](uint8_t index)
		{
			return data[index];
		}
		Buffer(const Buffer& c)
		{
			for(int i=0; i<COMMAND_LENGTH; ++i)
			{
				this->data[i] = c.data[i];
			}
		}
		Buffer() = default;
	private:
		static constexpr uint8_t COMMAND_LENGTH = 4U;
		uint8_t data[COMMAND_LENGTH];
};

#endif //__BUFFER_H__
