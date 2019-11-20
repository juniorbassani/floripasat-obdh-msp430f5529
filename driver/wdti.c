#include <driver/wdti.h>

void wdti_setup(char mode, char time2trigger) {
    wdti_hold_counter();                                /**< Stop counting                                  */
    WDTCTL = WDTPW + WDTHOLD + mode + time2trigger;     /**< Configure the mode and interval until trigger  */
    if (mode == INTERVAL)
        SFRIE1 |= WDTIE;
    wdti_release_counter();                             /**< Restart counting                               */
}

void wdti_reset_counter(void){
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;      /**< counter = 0                                    */
}

void wdti_hold_counter(void){
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;       /**< Stop the counting                              */
}

void wdti_release_counter(void){
    WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;    /**< Continue the counting                          */
}

void system_reboot(void){
    WDTCTL = 0x00;                                      /**< Force a reset                                  */
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
    system_reboot();
}
