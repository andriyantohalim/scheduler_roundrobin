/*
 * scheduler.h
 *
 *  Created on: 4 Sep 2019
 *      Author: Andriyanto Halim
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "process.h"

#define MAX_PROCESS 10

typedef struct SCHEDULER_t {
	uint8_t time_quantum;
	uint8_t total_time;
	uint8_t no_of_process;
	PROCESS_t *process[MAX_PROCESS];

	uint8_t curr_process;

	uint8_t tick;
} SCHEDULER_t;

#endif /* SRC_SCHEDULER_H_ */
