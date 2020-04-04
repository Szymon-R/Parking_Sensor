/* 
* I2CDriver.h
*
* Created: 29.03.2020 12:11:43
* Author: Szymon
*/


#ifndef __I2CDRIVER_H__
#define __I2CDRIVER_H__

#include "globals.h"
#include <stdint.h>

#include <avr/io.h>

enum class I2CFlag : uint8_t
{
	START = 0x08,
	REPEATED_START = 0x10,
	MT_SLA_ACK = 0x18,		//master transmitter slave adress acknowledge
	MT_SLA_NOT_ACK = 0x20,	//master transmitter slave adress not acknowledged
	MT_DATA_ACK = 0x28,		//master transmitter data acknowledge
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
	static I2CDriver& GetInstance() { return i2cDriver;}
		
private:
	I2CDriver() = default;
	static I2CDriver i2cDriver;
	I2CDriver(const I2CDriver &c) = delete;
	I2CDriver& operator=(const I2CDriver &c) = delete;
}; //I2CDriver

template<typename T>
bool I2CDriver::Transmit(uint8_t adress, T* dataPtr, uint8_t size)
{
	uint8_t counter = 0;
	const uint8_t* iter = reinterpret_cast<const uint8_t*>(dataPtr);
	const uint8_t* ending = iter + size*sizeof(uint8_t);
	if(Probe(adress) != true)
	{
		return false;
	}

	while(true)
	{
		TWDR = *iter;	//load data
		TWCR = (1<<TWINT) | (1<<TWEN);	//send data
		while (!(TWCR & (1<<TWINT)));	
		if(IsFlagEnabled(I2CFlag::MT_DATA_ACK) != true)
		{
			return false;
		}
		++iter;
		if(iter >= ending)
		{
			break;
		}
	}
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	return true;
}

#endif //__I2CDRIVER_H__
