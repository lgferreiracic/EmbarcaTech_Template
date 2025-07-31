#ifndef MY_TASKS_H
#define MY_TASKS_H

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

void init_queues(void);
void init_semaphores(void);
void init_tasks(void);

#endif // MY_TASKS_H
