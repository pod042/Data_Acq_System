/*
 * Module name: main.c
 *
 *
 * Description: This project will be used to implement
 *              general tests for the F28379D device.
 *              The final objective is to implement
 *              an ADC read routine on pair with an
 *              SPI communication routine, so as to
 *              verify the possibility of operating
 *              at max SPI speed in these conditions.
 *
 *
 * Current objective: Configure SPI slave
 *
 * Note: general configs based on TI examples
 */

/*******************************************************************************
*        Header Files                                                          *
*******************************************************************************/

#include "F28x_Project.h"
#include "Headers/system.h"
#include "Headers/EPWM.h"



int main(void)
{
    // Centralized initialization function
    SYS_GEN_CONFIG();

    // Inits EPWM
    EPWM_START();

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional):
    //
        for(;;)
        {
            asm ("    NOP");
        }
}
