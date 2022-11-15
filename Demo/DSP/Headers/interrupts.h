/*
 * interrupts.h
 *
 *  Created on: 19 de mai de 2022
 *      Author: rafae
 */

#include "F28x_Project.h"

#ifndef HEADERS_INTERRUPTS_H_
#define HEADERS_INTERRUPTS_H_

// Public interrupts

// EPWM-SOC-ADC Interrupt
interrupt void adca1_isr(void);

// SPI-A interrupts
interrupt void spia_rx_isr(void);
interrupt void spia_tx_isr(void);


// Public functions
void INTERRUPTS_INIT_VARIABLES(void); // Initializes variables used in interrupts



#endif /* HEADERS_INTERRUPTS_H_ */
