/* 
* I2CDriver.cpp
*
* Created: 29.03.2020 12:11:43
* Author: Szymon
*/


#include "I2CDriver.h"

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
		TWDR = *iter;
		TWCR = (1<<TWINT) | (1<<TWEN);
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
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		while (!(TWCR & (1<<TWINT)));		
	}
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	return true;	
}

bool I2CDriver::Probe(uint8_t adress)
{
	TWDR = adress;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return IsFlagEnabled(I2CFlag::MT_SLA_ACK);
}

void I2CDriver::Init()
{
	//Set clock frequency to 25kHz
	TWBR = 3;
	TWSR |= 1;	
}

bool I2CDriver::IsFlagEnabled(I2CFlag flag)
{
	return (TWSR & static_cast<uint8_t>(flag));
}
