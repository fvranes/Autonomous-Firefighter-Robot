#include "Includes.h"


TRISB = 0;               // All Port B pins are configured as outputs
TRISC = 2; 				 // Port C pin RC1 is set to an input, rest of the pins are outputs
TRISD = 0;               // All port D pins are configured as outputs
TRISE = 0;				 // All port E pins are configured as outputs
ADCON1 = 0;              // Voltage reference is brought to the RA3 pin.

unsigned char forward = 0xA;    ///Assigning values to the variables used to move the motors 
unsigned char backward = 0x5;
unsigned char left = 0x6;
unsigned char right = 0x9;
unsigned char rightwheelforward = 0x2;
unsigned char leftwheelforward = 0x8;
unsigned char linecount = 0;    ///for any counter variable, there must be a preset value
                     			 ///in this case, we want linecount to initially have a value of 0
unsigned int wallDetectionValue;
unsigned int flameDetectionValue;
unsigned int candleFound = 0;

///Introduction routine that turns all outputs off
void init()
{
	InitLCD();                      //initialize LCD
	PORTC.2 = 0;                   ///turns fan off
	PORTB = 0;                    ///turns motors off
	WriteStringToLCD("Start");       ///displays "Start" so that we know if the program has reached the "Intro" routine
	__delay_ms(10000);
}

void main(void)
{
    init();
    while (candleFound == 0) {
    	PORTB = forward;
    	WriteCommandToLCD(linecount+0x30);		 // Display linecount on LCD
    	_delay_ms(50);
		wallDetectionValue = GetADCValue(AN2);   // Read ADC value from AN2 pin
		wallDetectionValue = ((6787/(wallDetectionValue-3))-4)/5;     ///Equation converts analog values into a number in centimeters that
                                                                  ///we can use to turn our bot left if ?Leftdistance? exceeds a certain number
		if (wallDetectionValue < 10)
			wallCorrection(leftwheelforward);
		else if (wallDetectionValue > 15 && wallDetectionValue < 30)
			wallCorrection(rightwheelforward);
		else {
			_delay_ms(300);
			if (ninetyDegreeTurn(left) == 1) {
				if (scanMode() == 1) {
					candleFound = 1;
				}
				continue;
			}
			else {
				if (linecount == 3) {
					_delay_ms(2000);                 //if exiting room 3, go forward long enough so that the bot will not detect the opening on the left
				}
			}
		}

		Delay_ms(10);

		wallDetectionValue = GetADCValue(AN1);
		wallDetectionValue = ((6787/(wallDetectionValue-3))-4)/5;
                                                                 
		if (wallDetectionValue < 20)
			if (ninetyDegreeTurn(right) == 1)
				if (scanMode() == 1) {
					candleFound = 1;
				}
	}

	PORTB = forward;
	_delay_ms(1000);
	WriteStringToLCD("Extinguish");
	PORTB = 0;
	PORTC.2 = 1;
	flameDetectionValue = 1;    //set variable to 1 to get into the loop
	while (flameDetectionValue != 0) {
		flameDetectionValue = GetADCValue(AN2);    //keep reading the value until IR sensor cannot detect the candle
		_delay_ms(500);
	}
}

void wallCorrection(unsigned char wheelForward) {
	PORTB = wheelForward;
	_delay_ms(50);
	PORTB = forward;
	return;
}

int ninetyDegreeTurn(unsigned char direction) {
	PORTB = direction;
	_delay_ms(450);
	PORTB = forward;

	int i;
	for (i = 0; i < 50; i++) {
		if (PORTC.1 == 0)
			return 1;
		_delay_ms(10);
	}
	return 0;
}

int scanMode() {
	WriteStringToLCD("Scan Mode");
	PORTB = forward;
	_delay_ms(750);
	PORTB = right;
	int i;
	for (i = 0; i < 100; i++) {
		flameDetectionValue = GetADCValue(AN2);
		if (flameDetectionValue < 50)
			return 1;
		_delay_ms(10);
	}
	//no flame found at this point, exit room
	PORTB = forward;
	WriteStringToLCD("Exiting");
	linecount += 1;
	for (i = 0; i < 200; i++) {
		if (PORTC.1 == 0)
			break;
		_delay_ms(10);
	}
	return 0;
}