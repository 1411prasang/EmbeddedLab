#define  F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include <math.h> 

void buzzerBeep();
void lcdPrint(int,int,int);
int noOfOne(int);
void moveForward();
void moveBackward();
void moveLeft();
void moveRight();
void moveStop();
int measureDis();
unsigned char ADC_Conversion(unsigned char Ch);
unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading);
void adc_init();

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
	////////////////////////////////////////ADC Config Begin///////////////////////////////////////////
	DDRF	= 0x00;		// All Port F pins have to be set as input
	PORTF	= 0x00;		// Write suitable value in the PORT F register to make initial values to be "0"
	DDRK	= 0x00;	// All Port K pins have to be set as input
	PORTK	= 0x00;	// Write suitable value in the PORT K register to make initial values to be "0"
	////////////////////////////////////////ADC Config End/////////////////////////////////////////////
	int ledGraph=0x01;
	int one=0;
	int wordCounter=1;
	lcd_port_config();
	lcd_init();
	adc_init();
	lcd_home();
	
	while(1){
		while (ledGraph!=(0xFF))
		{
			PORTJ |= ledGraph;
			one++;
//			for(int i=0;i<one;i++){
//				buzzerBeep();
//				_delay_ms(2000);
//			}
			ledGraph = ledGraph << 1;
			ledGraph |= 0x01;
			int dis=measureDis();
			if(dis>=80 && dis<=130){
				moveLeft();
			}else{
				moveForward();
			}
			wordCounter++;
		}
		ledGraph=0x01;		
	}
}
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}
unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}
int measureDis(){
	int value = Sharp_GP2D12_estimation(ADC_Conversion(11));			//Stores Distance calsulated in a variable "value".
	lcd_numeric_value(2, 5, value, 3);  			//Prints Value Of Distanc in MM measured by Sharp Sensor.
    /*
	if(value>=100 && value<200)
	{
		lcd_string(1, 1, "Obstacle is Near:");
		lcd_string(2, 1, ":   ");	
	}
	if(value>=200 && value<500)
	{
		lcd_string(1, 1, "Obstacle is Far: ");
		lcd_string(2, 1, "    ");	
	}
	if(value>=500 && value<800)
	{
		lcd_string(1, 1, "Obstacle is Very ");
		lcd_string(2, 1, "Far:");		
	}
	*/
	return value;
}
void adc_init()
{
	ADCSRA = 0x00;		// Write suitable value in this register to disable ADC during initialization.
	ADCSRB = 0x00;		// Write suitable value in this register for initialization.
	ADMUX  = 0x20;		// Write appropriate value in the ADMUX register to select external Reference voltage (connected to AREF pin)
						// and left adjustment result (LAR) active.
	ACSR   = 0x80;		// Disable the analog comparator (optional)
	ADCSRA = 0x86;		// Write suitable value to this register to enable ADC and to select the required pre-scalar.
}
void moveForward(){
	lcdPrint(1,4,1);
	PORTA |= 0x06;
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