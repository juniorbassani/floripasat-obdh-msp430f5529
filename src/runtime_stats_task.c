/*
 * wdt_task.c
 *
 * Copyright (C) 2019, Universidade Federal de Santa Catarina
 *
 * This file is a modification of FloripaSat-OBDH.
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
 * \file runtime_stats_task.c
 *
 * \brief Task to receive and print information regarding
 * statistics of the application, such as processing
 * time consumed by each task
 *
 * \author Júnior Bassani
 *
 */

#include <string.h>
#include <src/runtime_stats_task.h>

static char stats[240] = {0};

void runtime_stats_task(void *pvParameters)
{
    uint16_t i;

    while (1) {
        /*
         * Wait for 195 seconds so that the main tasks
         * get executed in order to get their runtime statistics
         */
        vTaskDelay(20000);
        vTaskGetRunTimeStats(stats);

        // print collected data
        for (i = 0; i < strlen(stats); i++) {
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF = stats[i];
            __no_operation();
        }
    }

    vTaskDelete(NULL);
}
