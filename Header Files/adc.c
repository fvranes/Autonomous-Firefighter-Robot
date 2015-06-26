/* *****************************************************************************
 * File         : adc.c
 * Created on   : April 25, 2013, 9:39 AM
 * Description  : PIC16F877A ADC library source file
 * Notes        :
 * Author       : Anil C S
 * Website      : www.technoburst.net
 * Email        : anil@technoburst.net, technoburst@gmail.com
 ******************************************************************************/

#include <xc.h>
#include "adc.h"

void ADCPowerOn()
{
    ADCON0 = ADCON0 | 0b00000001;   //Sets ADON bit to power on ADC module
}

void ADCPowerOff()
{
    ADCON0 = ADCON0 & 0b11111110;   //Sets ADON bit to power on ADC module
}

void ADCSetClock(ADCClockDiv ClockDiv)
{
    switch (ClockDiv)
    {
        case FOSC_div_2:
            ADCON0bits.ADCS     = 0;
            ADCON1bits.ADCS2    = 0;
        case FOSC_div_8:
            ADCON0bits.ADCS     = 1;
            ADCON1bits.ADCS2    = 0;
        case FOSC_div_32:
            ADCON0bits.ADCS     = 2;
            ADCON1bits.ADCS2    = 0;
        case FRC:
            ADCON0bits.ADCS     = 3;
            ADCON1bits.ADCS2    = 0;
        case FOSC_div_4:
            ADCON0bits.ADCS     = 0;
            ADCON1bits.ADCS2    = 1;
        case FOSC_div_16:
            ADCON0bits.ADCS     = 1;
            ADCON1bits.ADCS2    = 1;
        case FOSC_div_64:
            ADCON0bits.ADCS     = 2;
            ADCON1bits.ADCS2    = 1;
        default:
            ADCON0bits.ADCS     = 0;
            ADCON1bits.ADCS2    = 0;
    }
}

void ADCConfigurePorts(unsigned char Value)
{
    if(Value<16)
        ADCON1bits.PCFG = Value;
    else
        ADCON1bits.PCFG = 0;
}

void ADCSetResultFormat(ADCResultFormat Format)
{
    if(Format == RightJustified)
        ADCON1bits.ADFM = 1;
    else
        ADCON1bits.ADFM = 0;
}

void ADCSetDefaults()
{
    ADCSetClock(FOSC_div_2);
    ADCSetResultFormat(RightJustified);
    ADCConfigurePorts(0);
}

void ADCSelectChannel(unsigned char ChannelNo)
{
    if(ChannelNo < 8)
        ADCON0bits.CHS = ChannelNo;
    else
        ADCON0bits.CHS = 0;
}

unsigned short ADCReadChannel()
{
    __delay_us(25); //Waits for the acquisition to complete

    ADCON0bits.GO = 1;   //Starts ADC conversion

    while (ADCON0bits.nDONE) continue;   //wait till ADC conversion is over

    return ( (ADRESH<<8) + ADRESL );
}