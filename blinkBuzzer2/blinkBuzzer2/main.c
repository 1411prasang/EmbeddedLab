
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000

void buzzerBeep();
int noOfOne(int);
int main(void)
{
	DDRJ=0xFF;						//Define all led's as output
	PORTJ=0x00;						//Define all led's as low in the beginning
	DDRC=0x08;						//Define 3rd pin of port C as output
	PORTC=0x00;						//Define all pins as low in the beginning of port C
	int ledGraph=0x01;
	int one=0;
	while (ledGraph!=(0xFF))
	{
		PORTJ |= ledGraph;
		one++;
		for(int i=0;i<one;i++){
			buzzerBeep();
			_delay_ms(2000);
		}
		ledGraph = ledGraph << 1;
		ledGraph |= 0x01;	
		_delay_ms(2000);	
	}
}

void buzzerBeep(){
	PORTC |= (0x08);
	_delay_ms(1000);
	PORTC &= (0x00);
	_delay_ms(1000);
}

