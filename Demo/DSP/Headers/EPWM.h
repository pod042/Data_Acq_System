/*
 * EPWM.h
 *
 *  Created on: 19 de mai de 2022
 *      Author: rafae
 */

#include "F28x_Project.h"

#ifndef HEADERS_EPWM_H_
#define HEADERS_EPWM_H_

// Defines
// Period of X counts
// EPWM_PERIOD = EPWM_hard_freq/(2*PWM_desired_freq)
// Example: if EPWM_hard_freq == 100 MHz (CLKDIV = 0 and HSPCLKDIV = 0 and ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0x1)
// for a desired freq. of 100 kHz: EPWM_PERIOD = 100 MHz / (2*100 kHz) = 500
// Note: it could be reduced to raise the number of points and thus the resolution
#define EPWM_PERIOD 500

// Public functions
void EPWM_CONFIG(void); // Configs EPWM GROUP A (not sure - needs revision)
void EPWM_START(void); // Starts EPWM pulses
void EPWM_CHANGE_DUTY_CYCLE(float D);


#endif /* HEADERS_EPWM_H_ */
