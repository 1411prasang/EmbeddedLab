#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

int main()
{
	UART_Init(9600);
	
	while(1)
	{
		UART_Printf("Welcome to AVR Serial Programming by ExploreEmbedded\n\r");
	}
	return (0);
}