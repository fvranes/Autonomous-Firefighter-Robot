#include "Includes.h"


void InitADC(void)
{
	ADCON1  = 0x80;	     // Make PORTA analog pins
						 // Also, Vref+ = 5v and Vref- = GND
	TRISA   = 0xff;      // Configure all port A pins as inputs
	ADCON0  = 0x81;		 // Turn on the A/D Converter
}


/*
   Channel name, it can be AN0, AN1, AN2, AN3, AN4
   AN5, AN6 or AN7 only, as defined in the LCD.h file.
   Channel is selected according to the pin you want to use
   in the ADC conversion. For example, use AN0 for RA0 pin.
   Similarly for RA1 pin use AN1 etc.
   
   10 bit ADC value is read from the pin and returned.
 */
unsigned int GetADCValue(unsigned char Channel)
{
	ADCON0 &= 0xc7;         // Clear Channel selection bits
	ADCON0 |= (Channel<<3); // Select channel pin as ADC input
    
    __delay_ms(10);         // Time for Acqusition capacitor 
							// to charge up and show correct value
	GO_nDONE  = 1;		    // Enable Go/Done

	while(GO_nDONE);        // Wait for conversion completion

	return ((ADRESH<<8)+ADRESL);   // Return 10 bit ADC value
}
