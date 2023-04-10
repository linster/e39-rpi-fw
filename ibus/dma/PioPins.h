//
// Created by stefan on 4/10/23.
//

#ifndef PICOTEMPLATE_PIOPINS_H
#define PICOTEMPLATE_PIOPINS_H

//These are the pins to the Lin Transceiver
#define LIN_TRANS_RX        27
#define LIN_TRANS_TX        29
#define LIN_ChipSelect      31
#define LIN_Fault           32

// UART 1 is the rx/tx link from PICO <-> PicoProbe with Debug logs.
// It's not used in the PIO programs.
#define UART1_RX
#define UART2_TX

// UART 2 is the rx/tx link from the PICO <-> PI with Raw IBUS data.
#define UART2_RX
#define UART2_TX

#endif //PICOTEMPLATE_PIOPINS_H
