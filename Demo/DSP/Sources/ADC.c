/*
 * ADC.c
 *
 *  Created on: 27 de abr de 2022
 *      Author: rafae
 */

#include "Headers/ADC.h"

// Public functions


// Configures and starts ADC
void ADC_CONFIG(void){

    EALLOW;
    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //
    //Set pulse positions to late
    //If 0: ADC interrupt occurs at the end of acq window
    //If 1: ADC interrupt occurs at the end of conversion (so after acq window)
    //Note: So if INTPULSEPOS==1, ADC SOC trigger -> acq window -> conversion -> interrupt
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //
    //power up the ADC
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    EDIS;
    return;
}

void ADC_SET_EPWM_ACQ_WINDOW(Uint16 channel){

    Uint16 acqps;

    //
    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    //
    //Select the channels to convert and end of conversion flag
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 for  ADCSOCA
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    EDIS;
}




// Sets continuous conversion mode
//void ADC_SET_CONTINUOUS_MODE(void);

