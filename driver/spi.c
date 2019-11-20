/*
 * spi.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file spi.c
 *
 * \brief This file manage the MSP430's registers of the SPI interfaces
 *
 * \author Arthur Semione
 *
 */

#include "spi.h"

#define DUMMY 0xFF

void port_mapping_uca0(void) {
    //PMAPPWD = 0x02D52;      /**< Enable Write-access to modify port mapping registers  */

    //PMAPCTL = PMAPRECFG;    /**< Allow reconfiguration during runtime                  */

    //P2MAP0 = PM_UCA0CLK;
    //P2MAP4 = PM_UCA0SIMO;
    //P2MAP5 = PM_UCA0SOMI;

    //PMAPPWD = 0;            /**< Disable Write-Access to modify port mapping registers */
}

// ##MODIFICADO##
void spi_clock_setup(void) {
    // Configure USCI_A0 for SPI operation
    //UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**
    //UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
    //UCA0CTLW0 |= UCSSEL__ACLK;                // ACLK
    //UCA0BR0 = 0x02;                           // /2
    //UCA0BR1 = 0;                              //
    //UCA0MCTLW = 0;                            // No modulation
    //UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
}

// ##MODIFICADO## -- Função adicionada
void spi1_clock_setup(void) {
    /*// Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt*/
}

// ##MODIFICADO##
void spi_setup(uint8_t interface) {

    switch(interface){
    case 0:
        BIT_SET(SPI0_SEL, SPI0_MOSI_PIN | SPI0_MISO_PIN );
        spi_clock_setup();
        //port_mapping_uca0();
        break;
    case 1:
        BIT_SET(SPI1_SEL, SPI1_MOSI_PIN | SPI1_MISO_PIN );
        //spi1_clock_setup();
        //BIT_SET(SPI1_MISO_REN, SPI1_MISO_PIN);                                  /**< Enable pull-up resistor */
        BIT_SET(SPI1_MISO_OUT, SPI1_MISO_PIN);
        break;
    }
}

void wait_for_buffer_empty(uint16_t base_address){
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCTXIFG));    /**< flag is set when data are sent/arrived */
}
void wait_for_data_sent(uint16_t base_address){
    while (!(HWREG8(base_address + OFS_UCAxIFG) & UCRXIFG));    /**< flag is set when data are sent/arrived */
}

uint8_t spi_tx(uint16_t base_address, uint8_t data) {

    wait_for_buffer_empty(base_address);
    HWREG8(base_address + OFS_UCAxTXBUF) = data;
    wait_for_data_sent(base_address);

	return (HWREG8(base_address + OFS_UCAxRXBUF));
}

void spi_tx_multiple(uint16_t base_address, uint8_t *data,uint8_t data_bytes_length) {

	uint8_t i;

	for(i=0; i < data_bytes_length; i++) {
		spi_tx(base_address, *data);
		data++;
	}
}

void spi_rx_multiple(uint16_t base_address, uint8_t *data, uint8_t data_bytes_length) {

	uint8_t i;

	for(i = 0; i < data_bytes_length; i++) {
		*data = spi_rx(base_address);
		data++;
    }

}


