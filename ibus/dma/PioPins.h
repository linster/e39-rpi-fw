//
// Created by stefan on 4/10/23.
//

#ifndef PICOTEMPLATE_PIOPINS_H
#define PICOTEMPLATE_PIOPINS_H

//These are the pins to the Lin Transceiver
//This is "Uart0"
//TODO rewire whateve GPIO is already on these pins to elsewhere.

//This block is with the numbers being actual pin numbers, not GP#
//#define UART0_LIN_TRANS_RX        22
//#define UART0_LIN_TRANS_TX        21
//#define LIN_ChipSelect      31
//#define LIN_Fault           32

//This block is with the numbers being GP numbers.
#define UART0_LIN_TRANS_RX        17
#define UART0_LIN_TRANS_TX        16
#define LIN_ChipSelect      26
#define LIN_Fault           27


// UART 1 is the rx/tx link from PICO <-> PicoProbe with raw IBUS output.
// It's not used in the PIO programs.
//We lose two bits out of the green channel.
//TODO BE CAREFUL WHEN SETTING UP VGA.
//https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf
// This is "UART 1"
//This block is with actual pin numbers, not GP#
//#define UART1_PICOPROBE_RX  12
//#define UART1_PICOPROBE_TX  11
//This block is with GP numbers
#define UART1_PICOPROBE_RX  9
#define UART1_PICOPROBE_TX  8

// UART 2 is the rx/tx link from the PICO <-> PI with Raw IBUS data.
//We won't be defining UART 2, because on the production board, we'll take those RX/TX lines
//and have Rpi4 GPIO read them. That leaves the Pico USB port as a convenient debug log viewer for the user.
//#define UART_PICO_PI_RX
//#define UART2_PICO_PI_TX

#endif //PICOTEMPLATE_PIOPINS_H
