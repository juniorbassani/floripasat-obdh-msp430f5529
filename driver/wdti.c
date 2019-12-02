#include <driver/wdti.h>

#ifdef DRIVERLIB
#include <driver/driverlib/wdt_a.h>
#endif

void wdti_setup(char mode, char time2trigger) {
    wdti_hold_counter();                                /**< Stop counting                                  */
#ifdef DRIVERLIB
    if (mode == INTERVAL)
        WDT_A_initIntervalTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_SMCLK, WDT_A_CLOCKDIVIDER_64);
    else
        WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_SMCLK, WDT_A_CLOCKDIVIDER_512K);
#else
    WDTCTL = WDTPW + WDTHOLD + mode + time2trigger;     /**< Configure the mode and interval until trigger  */
    if (mode == INTERVAL)
        SFRIE1 |= WDTIE;
#endif
    wdti_release_counter();                             /**< Restart counting                               */
}

void wdti_reset_counter(void){
#ifdef DRIVERLIB
    WDT_A_resetTimer(WDT_A_BASE);
#else
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;      /**< counter = 0                                    */
#endif
}

void wdti_hold_counter(void){
#ifdef DRIVERLIB
    WDT_A_hold(WDT_A_BASE);
#else
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;       /**< Stop the counting                              */
#endif
}

void wdti_release_counter(void){
#ifdef DRIVERLIB
    WDT_A_start(WDT_A_BASE);
#else
    WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;    /**< Continue the counting                          */
#endif
}

void system_reboot(void){
    WDTCTL = 0x00;                                      /**< Force a reset                                  */
}

// Watchdog Timer interrupt service routine
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
    system_reboot();
}
