
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

    while (1) 
    {
		PORTJ |= ledGraph;
		int one=noOfOne(PORTJ);		
		for(int i=0;i<one;i++){
			buzzerBeep();
		}
    }
}

void buzzerBeep(){
	PORTC |= (0x08);
	_delay_ms(500);
	PORTC &= (0x00);
	_delay_ms(500);
}
int noOfOne(int portj){
	int one=0;
	for(int i=0;i<8;i++){
		if(portj & (0x01)==0x01){		//1 is at LSB
			one++;
		}
	}
	return one;
}

