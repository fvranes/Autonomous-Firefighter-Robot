/* *****************************************************************************
 * File         : adc.h
 * Created on   : April 25, 2013, 9:39 AM
 * Description  : PIC16F877A ADC library header file
 * Notes        : Before including this header file _XTAL_FREQ macro should be defined
 * Author       : Anil C S
 * Website      : www.technoburst.net
 * Email        : anil@technoburst.net, technoburst@gmail.com
 ******************************************************************************/

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

//  Enumerator for clock conversion selection
typedef enum  {FOSC_div_2,FOSC_div_8,
                    FOSC_div_32,FRC,FOSC_div_4,
                    FOSC_div_16,FOSC_div_64}ADCClockDiv;

//  Enumerator for selection of result justification
typedef enum  {RightJustified,LeftJustified}ADCResultFormat;

/*
 * Funtion to power on the ADC module
 */
void ADCPowerOn();

/*
 * Funtion to power Off the ADC module
 */
void ADCPowerOff();

/**
 * Function to select result justification
 * @param Format : Either RightJustified or LeftJustified
 */
void ADCSetResultFormat(ADCResultFormat Format);

/**
 * Funtion to select clock conversion for ADC
 * @param ClockDiv : Any one of the constants defined in the enumerator ADCClockDiv
 */
void ADCSetClock(ADCClockDiv ClockDiv);

/**
 * Funtion to set ADC Port Configuration Control bits (PCFG3:PCFG0)
 * For more details on this ADC port configuration bits refer to data sheet
 * @param Value : Permitted values are 0 to 15
 */
void ADCConfigurePorts(unsigned char Value);

/**
 * Funtion to configure ADC in a default settings as given below
 * Clock conversion     : FOSC/2
 * Result format        : Right justified
 * ADC port configuration:All analog ports are configured as analod inputs.
 */
void ADCSetDefaults();

/**
 * Selects a ADC cjhannel from the 8 available ones
 * @param ChannelNo : Permitted values are 0 to 7
 */
void ADCSelectChannel(unsigned char ChannelNo);

/*
 * Funtion to read the channel input
 *
 * Return value is 10 bit ADC output. So minimum 10 bit variable
 * type is required. So we use unsigned short
 */


/**
 * Funtion to read the ADC channel input
 * @return Returns ADC result from the selected channel
 */
unsigned short ADCReadChannel();



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

