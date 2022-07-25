#define  F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

void buzzerBeep();
void lcdPrint(int,int,int);
int noOfOne(int);
void moveForward();
void moveBackward();
void moveLeft();
void moveRight();
void moveStop();

int main(void)
{
	DDRJ=0xFF;						//Define all led's as output
	PORTJ=0x00;						//Define all led's as low in the beginning
	DDRC=0x08;						//Define 3rd pin of port C as output
	PORTC=0x00;						//Define all pins as low in the beginning of port C
	DDRA |= 0x0F;					//Define Motor Pins as Output
	PORTA &= 0xF0;					//Define Motor Pins as LOW
	DDRL = DDRL | 0x18;				//Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18;			//PL3 and PL4 pins are for velocity control using PWM
	int ledGraph=0x01;
	int one=0;
	int wordCounter=1;
	lcd_port_config();
	lcd_init();
	lcd_home();
	
	while(1){
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
			
			///////////////One Circle
			moveForward();
			_delay_ms(7000);
			moveLeft();
			_delay_ms(4000);
			moveForward();
			_delay_ms(7000);
			moveLeft();
			_delay_ms(4000);
			moveForward();
			_delay_ms(7000);
			moveLeft();
			_delay_ms(4000);
			moveForward();
			_delay_ms(7000);
			wordCounter++;
		}
	}
}
void moveForward(){
	PORTA |= 0x06; 
	lcdPrint(1,4,1);	
}
void moveStop(){
	lcdPrint(1,4,5);
	PORTA &= 0x00;
}
void moveBackward(){
	lcdPrint(1,4,2);
	PORTA |= 0x09;
}
void moveLeft(){
	lcdPrint(1,4,3);
	PORTA |= 0x05;
}
void moveRight(){
	lcdPrint(1,4,4);
	PORTA |= 0x0A;
}
void buzzerBeep(){
	PORTC |= (0x08);
	_delay_ms(1000);
	PORTC &= (0x00);
	_delay_ms(1000);
}

void lcdPrint(int row,int coloumn,int direction){
	char forward[10]="FORWARD ";
	char backward[10]="BACKWARD ";
	char left[10]="LEFT ";
	char right[10]="RIGHT ";
	char nothing[10]="STOP ";
	char *ptr;
	switch(direction)
	{
		case 1:
			ptr=forward;
			break;	
		case 2:
			ptr=backward;
			break;	
		case 3:
			ptr=left;
			break;
		case 4:
			ptr=right;
			break;
		default :
			ptr=nothing;
			break;
	}
	while(*ptr!=' '){
		lcd_wr_char(row,coloumn,*ptr);
		coloumn++;
		ptr++;
	}
}