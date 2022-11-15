/*
 * system.c
 *
 *  Created on: 20 de abr de 2022
 *      Author: rafae
 */
#include <Headers/SPI_logger.h>
#include "Headers/system.h"
#include "Headers/interrupts.h"
#include "Headers/ADC.h"
#include "Headers/EPWM.h"
#include "Headers/GPIO.h"

/*******************************************************************************
*        Public Functions                                                      *
*******************************************************************************/
// SYS_GEN_CONFIG: configures PLL, ALL Periph. clocks and watchdog
// Note: taken from TI's F2837xD_SysCtrl.c file.
void SYS_GEN_CONFIG(void){

    // Configures PLL, watchdog and ALL periph. clocks

    /* PLL CONFIG (_LAUNCHXL_F28379D DEFINED -> 200 MHz):
    // Initialize the PLL control: SYSPLLMULT and SYSCLKDIVSEL.
    //
    // Defined options to be passed as arguments to this function are defined
    // in F2837xD_Examples.h.
    //
    // Note: The internal oscillator CANNOT be used as the PLL source if the
    // PLLSYSCLK is configured to frequencies above 194 MHz.
    //
    //  PLLSYSCLK = (XTAL_OSC) * (IMULT + FMULT) / (PLLSYSCLKDIV)
    //
    #ifdef _LAUNCHXL_F28379D
    InitSysPll(XTAL_OSC,IMULT_40,FMULT_0,PLLCLK_BY_2);
    #else
    InitSysPll(XTAL_OSC, IMULT_20, FMULT_0, PLLCLK_BY_2);
    #endif // _LAUNCHXL_F28379D
    */
    InitSysCtrl();
    // Sets LSPCLK to 200 MHz
    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0b000;
    EDIS;

    // Step 2. Initialize GPIO:
    //GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;

    // This example function is found in the F2837xD_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    InitGpio();
    // Also sets P61 (SPI-A cs) to be an output
    GPIO_SetupPinOptions(SPI_CS_PIN, GPIO_OUTPUT, GPIO_OPENDRAIN);
    GPIO_WritePin(SPI_CS_PIN, 1);
    //GPIO_SetupPinOptions(SPI_EN_PIN, GPIO_OUTPUT, GPIO_OPENDRAIN);
    //GPIO_WritePin(SPI_EN_PIN, 0);

    //CpuSysRegs.PCLKCR2.bit.EPWM1=1;

    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    // Step 4. Initialize SPI
    GPIO_SPI_HS_CONFIG();
    SPI_INIT();
    // Register 4 16 bit samples
    SPI_register_sample(16);
    SPI_register_sample(16);
    SPI_register_sample(16);
    SPI_register_sample(16);

    // Sets up comm. link and stops program in case of failure
    if(SPI_comm_setup() == SPI_MODULE_FAILURE){
        asm("     ESTOP0");  //Test failed!! Stop!
        for(;;);
    }

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    //
    InitPieVectTable();

    // ISRs used (ADC SOC on ePWM) - Note: write other functions later
    EALLOW;
    PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt 1
    PieVectTable.SPIA_RX_INT = &spia_rx_isr; // function for commanding the cs line
    //PieVectTable.SPIA_TX_INT = &spia_tx_isr; // FIFO overflow
    EDIS;

    // Note: for now, cotrol LED brightness function is written in ADC interrupt
    // Configures GPIO

    // Configures ADCA
    ADC_CONFIG();
    // Configures ePWM
    EPWM_CONFIG();

    // Sets ADCA to convert on start of EPWM1 - ch0 pulse
    ADC_SET_EPWM_ACQ_WINDOW(0);

    // Initializes interrupt variables
    INTERRUPTS_INIT_VARIABLES();






    //while(1);

    // TO BE DONE...


    // Step 5. Init SPI Interrupts (to send dummy data)

    // enable PIE interrupt
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;     // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER6.bit.INTx1 = 1;     // Enable PIE Group 6, INT 1 (SPI RX)
    //PieCtrlRegs.PIEIER6.bit.INTx2 = 1;     // Enable PIE Group 6, INT 2 (SPI TX)


    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    IER |= M_INT1; //Enable group 1 interrupts
    IER |= M_INT6; //Enable group 6 interrupts
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM




    //
    // sync ePWM
    //
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;



    return;
}



/*******************************************************************************
 *       Private Functions                                                     *
 ******************************************************************************/


