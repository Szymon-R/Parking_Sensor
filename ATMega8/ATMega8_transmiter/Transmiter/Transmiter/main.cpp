/*
 * Transmiter.cpp
 *
 * Created: 09.05.2020 21:36:09
 * Author : Szymon
 */ 
 #include "globals.h"
 #include "UartObject.h"
 #include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#define  Trigger_pin	PC0	/* Trigger pin */


int TimerOverflow = 0;
UARTObject uart1;

int main(void)
{

	uart1.Init();
    /* Replace with your application code */
	char string[10];
	long count;
	double distance;
	
	DDRC = 0x01;		/* Make trigger pin as output */
	PORTB = 0x01;		/* Turn on Pull-up */
		
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */
	uart1.Transmit("Start");
	char output[50];
	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTC|= (1 << Trigger_pin);
		_delay_us(10);
		PORTC &= (~(1 << Trigger_pin));
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = 17.1500 * (double)count * 0.001;
		//ftoa(distance, output);
		sprintf(output, "%f", distance);
		uart1.Transmit(output, 50);
		uart1.Transmit("\n");
		_delay_ms(200);
	}
}

ISR(TIMER1_OVF_vect)
{
	//uart1.Transmit("Interrupt\n");
	TimerOverflow++;	/* Increment Timer Overflow count */
}
/*
void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}*/