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

	process_initialize(&P0, 0, 5, &PROCESS_0);
	process_initialize(&P1, 3, 7, &PROCESS_1);
	process_initialize(&P2, 6, 4, &PROCESS_2);

	scheduler_initialize(&RoundRobin, 5);

	scheduler_register_process(&RoundRobin, &P0);
	scheduler_register_process(&RoundRobin, &P1);
	scheduler_register_process(&RoundRobin, &P2);

	// calculate total time. you can think of this as one period
	scheduler_calculate_total_time(&RoundRobin);

	// create another function to translate all the timings data from the process and map into scheduler tick.
	// working with scheduler tick is easier

	printf("Round robin total time : %d\n", RoundRobin.total_time);
	printf("Round robin total process : %d\n", RoundRobin.no_of_process);




	while(1) //ignore this. just used for repeating
	{
		// Time Quantum emulation. In MCU, this is the while(1) loop
		for(unsigned int i = 0; i < ((1 << 16) - 1); i ++)
		{
			// execute process. Function should be placed here.
//			RoundRobin.process[RoundRobin.curr_process]->process_function();

		}

		// execute process
		RoundRobin.process[RoundRobin.curr_process]->process_function();
//		printf("Current Process index:  %d\n\n", RoundRobin.curr_process);

		// Call Timer Interrupt after some time
		TimerISR();
	}




	return 0;
}

void TimerISR(void)
{
	RoundRobin.tick ++;
	RoundRobin.process[RoundRobin.curr_process]->tick ++;

	// check scheduler current tick against arrival time

	//



	if(RoundRobin.tick >= RoundRobin.time_quantum)
	{
		RoundRobin.tick = 0;

		// manage unfinished processes

		RoundRobin.curr_process ++;

		if(RoundRobin.curr_process >= RoundRobin.no_of_process)
		{
			RoundRobin.curr_process = 0;
		}


	}
	else
	{

	}




}

