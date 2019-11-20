#ifndef RUNTIME_STATS_TASK_H_
#define RUNTIME_STATS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

#define RUNTIME_STATS_TASK_PRIORITY        (configMAX_PRIORITIES - 1)
#define RUNTIME_STATS_TASK_PERIOD_MS       300000
#define RUNTIME_STATS_TASK_PERIOD_TICKS    (RUNTIME_STATS_TASK_PERIOD_MS / portTICK_PERIOD_MS)

/**
 * \fn runtime_stats_task( void *pvParameters )
 *
 * Task to receive and print information regarding
 * statistics of the application, such as processing
 * time consumed by each task
 *
 * \param pvParameters Not used
 * \return None
 */
void runtime_stats_task( void *pvParameters );

#endif /* RUNTIME_STATS_TASK_H_ */
