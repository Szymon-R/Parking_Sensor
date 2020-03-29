/* 
* I2CDriver.h
*
* Created: 29.03.2020 12:11:43
* Author: Szymon
*/


#ifndef __I2CDRIVER_H__
#define __I2CDRIVER_H__

#include "globals.h"

#include <avr/io.h>

enum class I2CFlag : uint8_t
{
	START = 0x08,
	REPEATED_START = 0x10,
	MT_SLA_ACK = 0x18,//master transmitter slave dress acknowledge
	MT_SLA_NOT_ACK = 0x20,
	MT_DATA_ACK = 0x28,
	MT_DATA_NOT_ACK = 0x30
};


class I2CDriver
{
public:
	template<typename T>
	bool Transmit(uint8_t adress, T* dataPtr, uint8_t size);
	bool Probe(uint8_t adress);
	void Init();
	bool IsFlagEnabled(I2CFlag flag);
private:
	I2CDriver( const I2CDriver &c );
	I2CDriver& operator=( const I2CDriver &c );

}; //I2CDriver

#endif //__I2CDRIVER_H__
