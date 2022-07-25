
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define F_CPU 16000000

void buzzerBeep();
int lcdPrint(int,int,int);
int noOfOne(int);

int main(void)
{
	DDRJ=0xFF;						//Define all led's as output
	PORTJ=0x00;						//Define all led's as low in the beginning
	DDRC=0x08;						//Define 3rd pin of port C as output
	PORTC=0x00;						//Define all pins as low in the beginning of port C
	int ledGraph=0x01;
	int one=0;
	int posiPass=0;
	lcd_port_config();
	lcd_init();
	lcd_home();
	
	
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
		posiPass=lcdPrint(1,1,posiPass);
		
		_delay_ms(2000);
	}
	
}

void buzzerBeep(){
	PORTC |= (0x08);
	_delay_ms(1000);
	PORTC &= (0x00);
	_delay_ms(1000);
}

int lcdPrint(int row,int coloumn,int startPosi){
	char description[50]=" PRASANG AGARWAL 19010201 ECE";
	int i=startPosi+1;
	coloumn=startPosi+1;
	while(description[i]!=' '){
		if(coloumn>16 && coloumn<32){
			coloumn=0;
			row=2;
			}else{
			row=1;
			lcd_clear();
		}
		lcd_wr_char(row,coloumn,description[i]);
		i++;
		coloumn++;		
	}
	return i;	
}