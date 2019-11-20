#include <msp430.h>

#include "include/obdh.h"

int main( void )
{
    setup_hardware();
    create_tasks();

    vTaskStartScheduler();

    return 0;
}
