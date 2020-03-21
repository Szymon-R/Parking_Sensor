#include <avr/io.h>
#include <util/delay.h>

class UARTObject
{
	struct Parameters
	{
		static constexpr int COMMAND_LENGTH = 10U;
		static constexpr int BAUD = 9600;
		static constexpr DATA_LENGTH = 8;
	};
	public:
		Init();
	private:
		
		
		uint8_t receiveBuffer[COMMAND_LENGTH];	
	
};