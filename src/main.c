/*
 * main.c
 *
 *  Created on: 4 Sep 2019
 *      Author: Andriyanto Halim
 */

#include "main.h"

void TimerISR(void);

SCHEDULER_t RoundRobin;
PROCESS_t P0;
PROCESS_t P1;
PROCESS_t P2;



void process_initialize(PROCESS_t *process, uint8_t arrival_time, uint8_t burst_time, void(*process_function)(void))
{
	process->arrival_time = arrival_time;
	process->burst_time = burst_time;
	process->process_function = process_function;
}

void scheduler_initialize(SCHEDULER_t *scheduler, uint8_t time_quantum)
{
	scheduler->time_quantum = time_quantum;
	scheduler->no_of_process = 0; // Initialize to 0
}

void scheduler_register_process(SCHEDULER_t *scheduler, PROCESS_t *process)
{
	scheduler->process[scheduler->no_of_process] = process;
	scheduler->no_of_process += 1;
}

void scheduler_calculate_total_time(SCHEDULER_t *scheduler)
{
	scheduler->total_time = 0;

	for(unsigned int i = 0; i < scheduler->no_of_process; i ++)
	{
		scheduler->total_time += scheduler->process[i]->burst_time;
	}
}



int main()
{
	printf("Hello world\n");


	scheduler_initialize(&RoundRobin, 5);

	process_initialize(&P0, 0, 5, &PROCESS_0);
	process_initialize(&P1, 3, 7, &PROCESS_1);
	process_initialize(&P2, 6, 4, &PROCESS_0);

	scheduler_register_process(&RoundRobin, &P0);
	scheduler_register_process(&RoundRobin, &P1);
	scheduler_register_process(&RoundRobin, &P2);





	scheduler_calculate_total_time(&RoundRobin);

	printf("Round robin total time : %d\n", RoundRobin.total_time);
	printf("Round robin total process : %d\n", RoundRobin.no_of_process);




//	while(1) //ignore this
//	{
//
//		// Time Quantum emulation
//		for(unsigned int i = 0; i < ((1 << 16) - 1); i ++)
//		{
//			RoundRobin.process[RoundRobin.curr_process]->process_function();
//			printf("Current Process index:  %d\n", RoundRobin.curr_process);
//
//		}
//
//
//		RoundRobin.tick ++;
//
//		if(RoundRobin.tick >= RoundRobin.time_quantum)
//		{
//			RoundRobin.tick = 0;
//
//			TimerISR();
//		}
//	}




	return 0;
}

void TimerISR(void)
{
	printf("Hello world from Timer ISR EMULATION\n");

	RoundRobin.tick ++;

	if(RoundRobin.tick >= RoundRobin.time_quantum)
	{
		RoundRobin.tick = 0;

		// manage unfinished processes
	}
	else
	{

	}


	RoundRobin.curr_process ++;

	if(RoundRobin.curr_process >= RoundRobin.no_of_process)
	{
		RoundRobin.curr_process = 0;
	}

}

