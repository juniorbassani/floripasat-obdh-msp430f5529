/*
 * adc.c
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
 * \file adc.c
 *
 * \brief Functions of MSP430's ADC12 peripheral
 *
 * \author Elder Tramontin
 *
 */

#include <driver/adc.h>

void adc_setup(void) {
#ifdef _SIM
    P1SEL1 |= BIT1;                           // Configure P1.1 for ADC
    P1SEL0 |= BIT1;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;        // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                     // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_1;                // A1 ADC input select; Vref=AVCC
    ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
#else
    //  Internal temperature reading setup
    REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
                                            // ADC12_A ref control registers
    ADC12CTL0 = ADC12MSC | ADC12SHT0_15 /*| ADC12REFON*/ | ADC12ON; /**< Vref+ = 3.0V, Vref- = 0V   */

    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;                       /**< enable sample timer        */

    #if HAL_VERSION == HAL_V2_0
    BIT_SET(ADC_SEL, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN
                      | OBDH_CURRENT_ADC_PIN | ADC_Vref_TEMP_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                 /**< Sun sensor -X      */
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;                 /**< Sun sensor -Y      */
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;                 /**< Sun sensor -Z      */
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                 /**< OBDH current-sense */
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                 /**< Vref temp. sensor  */
    ADC12MCTL5 = ADC12SREF_2 | ADC12INCH_5;                 /**< OBDH Voltage-sense */
    ADC12MCTL6 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     /**< Temperature sensor */
    #endif

    #if HAL_VERSION == HAL_V2_1
    BIT_SET(ADC_SEL0, MAIN_ADC_PIN);
    BIT_SET(ADC_SEL1, MAIN_ADC_PIN);

    BIT_SET(ADC_SEL0, (X_SUNSEN_ADC_PIN | Y_SUNSEN_ADC_PIN | Z_SUNSEN_ADC_PIN
                      | OBDH_CURRENT_ADC_PIN | VCC_3V3_PIN));

    ADC12MCTL0 = ADC12SREF_2 | ADC12INCH_0;                 /**< Sun sensor -X      */
    ADC12MCTL1 = ADC12SREF_2 | ADC12INCH_1;                 /**< Sun sensor -Y      */
    ADC12MCTL2 = ADC12SREF_2 | ADC12INCH_2;                 /**< Sun sensor -Z      */
    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                 /**< OBDH current-sense */
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                 /**< OBDH Voltage-sense */
    ADC12MCTL5 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     /**< Temperature sensor */
    #endif
#endif

    __delay_cycles(2000);       /**< Allow ~100us (at default UCS settings) for REF to settle */
    ADC12CTL0 |= ADC12ENC;
}


uint16_t adc_read(uint8_t channel){
    while(ADC12CTL1 & ADC12BUSY);
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

#ifdef _SIM
    uint8_t i = 0;
switch(channel){
    case 0:
        while (i++ < 50);
        return ADC12MEM0;
    case 1:
        while (i++ < 50);
        return ADC12MEM1;
    case 2:
        while (i++ < 50);
        return ADC12MEM2;
    case 3:
        while (i++ < 50);
        return ADC12MEM3;
    case 4:
        while (i++ < 50);
        return ADC12MEM4;
    case 5:
        while (i++ < 50);
        return ADC12MEM5;
    case 6:
        while (i++ < 50);
        return ADC12MEM6;
    case 7:
        while (i++ < 50);
        return ADC12MEM7;
    case 8:
        while (i++ < 50);
        return ADC12MEM8;
    case 9:
        while (i++ < 50);
        return ADC12MEM9;
    case 10:
        while (i++ < 50);
        return ADC12MEM10;
    case 11:
        while (i++ < 50);
        return ADC12MEM11;
    case 12:
        while (i++ < 50);
        return ADC12MEM12;
    case 13:
        while (i++ < 50);
        return ADC12MEM13;
    case 14:
        while (i++ < 50);
        return ADC12MEM14;
    case 15:
        while (i++ < 50);
        return ADC12MEM15;
    }
#else
    switch(channel){
    case 0:
        while (!(ADC12IV_ADC12OVIFG & BIT0));
        return ADC12MEM0;
    case 1:
        while (!(ADC12IV_ADC12OVIFG & BIT1));
        return ADC12MEM1;
    case 2:
        while (!(ADC12IV_ADC12OVIFG & BIT2));
        return ADC12MEM2;
    case 3:
        while (!(ADC12IV_ADC12OVIFG & BIT3));
        return ADC12MEM3;
    case 4:
        while (!(ADC12IV_ADC12OVIFG & BIT4));
        return ADC12MEM4;
    case 5:
        while (!(ADC12IV_ADC12OVIFG & BIT5));
        return ADC12MEM5;
    case 6:
        while (!(ADC12IV_ADC12OVIFG & BIT6));
        return ADC12MEM6;
    case 7:
        while (!(ADC12IV_ADC12OVIFG & BIT7));
        return ADC12MEM7;
    case 8:
        while (!(ADC12IV_ADC12OVIFG & BIT8));
        return ADC12MEM8;
    case 9:
        while (!(ADC12IV_ADC12OVIFG & BIT9));
        return ADC12MEM9;
    case 10:
        while (!(ADC12IV_ADC12OVIFG & BITA));
        return ADC12MEM10;
    case 11:
        while (!(ADC12IV_ADC12OVIFG & BITB));
        return ADC12MEM11;
    case 12:
        while (!(ADC12IV_ADC12OVIFG & BITC));
        return ADC12MEM12;
    case 13:
        while (!(ADC12IV_ADC12OVIFG & BITD));
        return ADC12MEM13;
    case 14:
        while (!(ADC12IV_ADC12OVIFG & BITE));
        return ADC12MEM14;
    case 15:
        while (!(ADC12IV_ADC12OVIFG & BITF));
        return ADC12MEM15;
    }
#endif

    return 0;
}
