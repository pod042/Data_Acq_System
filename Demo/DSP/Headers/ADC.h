/*
 * ADC.h
 *
 *  Created on: 27 de abr de 2022
 *      Author: rafae
 */

#include "F28x_Project.h"

#ifndef HEADERS_ADC_H_
#define HEADERS_ADC_H_

// Public parameters
#define ADC_RESULTS_BUFFER_SIZE 256

// Global variables
extern Uint16 ADC_CHA_RESULTS[ADC_RESULTS_BUFFER_SIZE];

// Public functions
void ADC_CONFIG(void); // Configures and starts ADC
void ADC_SET_EPWM_ACQ_WINDOW(Uint16 channel);


//void ADC_SET_CONTINUOUS_MODE(void);




#endif /* HEADERS_ADC_H_ */
