/*
 * process.h
 *
 *  Created on: 4 Sep 2019
 *      Author: Andriyanto Halim
 */

#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct PROCESS_t {
	void (*process_function)(void);
	uint8_t arrival_time;
	uint8_t burst_time;

	uint8_t tick;
} PROCESS_t;



void PROCESS_0(void);
void PROCESS_1(void);
void PROCESS_2(void);

#endif /* SRC_PROCESS_H_ */
