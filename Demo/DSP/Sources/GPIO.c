/*
 * GPIO.c
 *
 *  Created on: 19 de mai de 2022
 *      Author: rafae
 */

#include "Headers/GPIO.h"

// --- Functions ---
void GPIO_CONFIG(void);

void GPIO_SPI_HS_CONFIG(void){

    // Sets SPI to High Speed pins
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //

    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0; // SPI MOSI
    GpioCtrlRegs.GPBPUD.bit.GPIO59 = 1; // SPI MISO
    GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0; // SPI CLK
    GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0; // SPI CS

//   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
//   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   // Enable pull-up on GPIO5 (SPISIMOA)
//   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up on GPIO17 (SPISOMIA)
//   GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   // Enable pull-up on GPIO3 (SPISOMIA)
//   GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)
//   GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;  // Enable pull-up on GPIO19 (SPISTEA)

    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //

    GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // SPI MOSI
    GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // SPI MISO
    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // SPI CLK
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // SPI CS

//   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
//   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
//   GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
//   GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    //
    //Configure SPI-A pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be SPI functional
    // pins.
    // Comment out other unwanted lines.
    //

    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3; // SPI MOSI
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 3; // SPI MISI
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 3; // SPI CLK
    //GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 3; // SPI CS

    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 3; // SPI MOSI
    GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 3; // SPI MISI
    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 3; // SPI CLK
    //GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 3; // SPI CS

//    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
//  GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
// GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;

}
