/* 
* I2CDriver.cpp
*
* Created: 29.03.2020 12:11:43
* Author: Szymon
*/


#include "I2CDriver.h"

bool I2CDriver::Probe(uint8_t adress)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	
	while (!(TWCR & (1<<TWINT)));
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

I2CDriver I2CDriver::i2cDriver;