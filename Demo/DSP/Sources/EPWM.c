/*
 * EPWM.c
 *
 *  Created on: 19 de mai de 2022
 *      Author: rafae
 */

#include "Headers/EPWM.h"


// Public functions

// Configures EPWM to generate SOC pulses when counter equals 0 (start of period)
// Notes: PWM frequency can be controlled by changing frequency prescaler register bits and total count number
// Frequency related registers: PERCLKDIVSEL, TBCTL
// Count number related registers: TBPRD
// General config register: TBCTL
void EPWM_CONFIG(void)
{

    InitEPwm1Gpio();

    // Configures PWM operation
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Triangular goes up then down;
    //Note: CTRMODE is changed later (freeze then start, so irrelevant here, but good for understanding of configs)

    EPwm1Regs.TBCTL.bit.PHSDIR = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Clear PWM1A on event A, up count
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;              // Set PWM1A on event A, down count
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0

    // Configures PWM frequency
    EPwm1Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to EPWMCLK: if both
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;           // HSPCLKDIV and CLKDIV == 1, then
                                                    // set PWM hardware freq. to SYSCLK/2

    EPwm1Regs.TBPRD = EPWM_PERIOD;                  // Set period to 500 counts -> PWM_frequency of 100 kHz

    // Setup shadow register load on ZERO
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configures SOCA interrupt
    EALLOW;
    // Assumes ePWM clock is already enabled
    // The function InitSysCtrl enables the clock
    // However, the ePWM clock is derived from sysclk (200 MHz)
    // with a reset divider of 2 (it can be configured to 1)
    // How to configure to 1: ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EPwm1Regs.ETSEL.bit.SOCAEN    = 0;    // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL    = 1;   // Select SOC on start of period
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st SOC event - can be up to 3 if configured
    EPwm1Regs.CMPA.bit.CMPA = 250;        // Set compare A value to 250 counts (50% duty cycle)
    EPwm1Regs.TBCTL.bit.CTRMODE = 3;      // freeze counter
    EDIS;

}

// Starts EPWM pulses and SOC triggers
// For now is set to just count up
void EPWM_START(void){

    //start ePWM
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  //enable SOCA
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; //unfreeze, and enter up count mode

}

void EPWM_CHANGE_DUTY_CYCLE(float D){

    EPwm1Regs.CMPA.bit.CMPA = D*EPWM_PERIOD;
}
