#include <adc.h>
#include <pic.h>
#include <htc.h>
#include <lcd.h>
#include <stdlib.h>

// LCD module connections
#define RS PORTD.0
#define EN PORTD.2
#define D4 PORTD.4
#define D5 PORTD.5
#define D6 PORTD.6
#define D7 PORTD.7
// End LCD module connections

TRISA = 0xFF;               // All port A pins are configured as inputs
TRISB = 0;               // Port B pins RB7 and RB6 are configured as outputs
TRISC = 0b00000010;
TRISD = 0;                  // All port D pins are configured as outputs
TRISE = 0;
ADCON1 = 0;             // Voltage reference is brought to the RA3 pin.

unsigned char forward = ob00001010;    ///Assigning values to the variables used to move the motors 
unsigned char backward = 0b00000101;
unsigned char left = 0b00000110;
unsigned char right = 0b00001001;
unsigned char rightwheelforward = 0b00000010;
unsigned char leftwheelforward = 0b00001000;
unsigned char linecount = 0;    ///for any counter variable, there must be a preset value
                     			 ///in this case, we want linecount to initially have a value of 0
unsigned int wallDetectionValue;
unsigned int flameDetectionValue;
unsigned int candleFound = 0;

///Introduction routine that turns all outputs off
void init()
{
	Lcd4_Init();
	PORTC.2 = 0;                   ///turns fan off
	PORTB = 0;                    ///turns motors off
	Lcd_Out(1, 1, "Start");       ///displays ?start? so that we know if the program has reached the ?Intro? routine
	_delay_ms(10000);              ///this routine will last 10 seconds, which is enough time to carefully set bot into the maze
}

int main(void)
{
    init();
    while (candleFound == 0) {
    	PORTB = forward;
    	Lcd_Out(1, 1, linecount);
    	_delay_ms(50);
		wallDetectionValue = ADC_Read(2);
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

		wallDetectionValue = ADC_Read(1);
		wallDetectionValue = ((6787/(wallDetectionValue-3))-4)/5;     ///Equation converts analog values into a number in centimeters that
                                                                  ///we can use to turn our bot left if ?Leftdistance? exceeds a certain number
		if (wallDetectionValue < 20)
			if (ninetyDegreeTurn(right) == 1)
				if (scanMode() == 1) {
					candleFound = 1;
				}
	}
	PORTB = forward;
	_delay_ms(1000);
	Lcd_Out(1, 1, "Extinguish");
	PORTB = 0;
	PORTC.2 = 1;
	flameDetectionValue = ADC_Read(2);
	while (flameDetectionValue != 0) {
		flameDetectionValue = ADC_Read(2);
		_delay_ms(500);
	}
	return 0;
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
	Lcd_Out(1, 1, "Scan Mode");
	PORTB = forward;
	_delay_ms(750);
	PORTB = right;
	int i;
	for (i = 0; i < 100; i++) {
		flameDetectionValue = ADC_Read(2);
		if (flameDetectionValue < 50)
			return 1;
		_delay_ms(10);
	}
	//no flame found at this point, exit room
	PORTB = forward;
	Lcd_Out(1, 1, "Exit");
	linecount += 1;
	for (i = 0; i < 200; i++) {
		if (PORTC.1 == 0)
			break;
		_delay_ms(10);
	}
	return 0;
}