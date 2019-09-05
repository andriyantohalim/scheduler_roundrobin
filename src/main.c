/*
 * main.c
 *
 *  Created on: 4 Sep 2019
 *      Author: Andriyanto Halim
 */

#include "main.h"

void TimerISR(void);

SCHEDULER_t RR;
PROCESS_t P0;
PROCESS_t P1;
PROCESS_t P2;
PROCESS_t P3;
PROCESS_t P4;


void process_initialize(PROCESS_t *process, uint8_t arrival_time, uint8_t burst_time, void(*process_function)(void))
{
	process->process_function = process_function;

	process->arrival_time = arrival_time;
	process->burst_time = burst_time;
	process->process_tick = 0;
}

void scheduler_initialize(SCHEDULER_t *scheduler, uint8_t time_quantum)
{
	scheduler->time_quantum = time_quantum;
	scheduler->no_of_process = 0; // Initialize to 0
	scheduler->curr_process = 0; // Initialize to 0
	scheduler->scheduler_tick = 0; // Initialize to 0
}

void scheduler_register_process(SCHEDULER_t *scheduler, PROCESS_t *process)
{
	scheduler->process[scheduler->no_of_process] = process;
	scheduler->no_of_process += 1;
}

void scheduler_calculate_total_time(SCHEDULER_t *scheduler)
{
	scheduler->total_time = 0;

	for(uint8_t i = 0; i < scheduler->no_of_process; i ++)
	{
		scheduler->total_time += scheduler->process[i]->burst_time;
	}
}



int main()
{
	printf("Hello world\n");

	process_initialize(&P0, 0, 3, &PROCESS_0);
	process_initialize(&P1, 2, 2, &PROCESS_1);
	process_initialize(&P2, 2, 2, &PROCESS_2);

	scheduler_initialize(&RR, 5);

	scheduler_register_process(&RR, &P0);
	scheduler_register_process(&RR, &P1);
	scheduler_register_process(&RR, &P2);


	scheduler_calculate_total_time(&RR); // calculate total time. you can think of this as one scheduler period

	// create another function to translate all the timings data from the process and map into scheduler tick.
	// working with scheduler tick is easier


	printf("Round robin total time : %d\n", RR.total_time);
	printf("Round robin total process : %d\n", RR.no_of_process);


	while(1) //ignore this. just used for repeating
	{
		// Time Quantum emulation. In MCU, this is the while(1) loop
		for(uint16_t i = 0; i < ((1 << 16) - 1); i ++)
		{
			// execute process. Function should be placed here.
//			RR.process[RR.curr_process]->process_function();

		}

		// execute process. put here just to slow it down
		RR.process[RR.curr_process]->process_function();
//		printf("Current Process index:  %d\n\n", RR.curr_process);

		// Call Timer Interrupt after some time
		TimerISR();
	}




	return 0;
}

void TimerISR(void)
{
	RR.scheduler_tick ++; // increase scheduler tick at every timer ISR
	printf("Scheduler tick : %d\t", RR.scheduler_tick);

	if(RR.scheduler_tick < RR.total_time)
	{

		for(uint8_t i = 0; i < RR.no_of_process; i++)
		{
			// ensure no early arrival
			if(RR.scheduler_tick >= RR.process[i]->arrival_time)
			{
				// ensure process does not exceed burst time
				if(RR.process[i]->process_tick < RR.process[i]->burst_time)
				{
					// for processes whose burst time less than or equal to time quantum
					if(RR.process[i]->burst_time <= RR.time_quantum)
					{
						// ensure process does not exceed time quantum
						if(RR.process[i]->process_tick < RR.time_quantum)
						{
							RR.curr_process = i; // end result we want
							RR.process[RR.curr_process]->process_tick ++;
							break;
						}

					}

					// for processes whose burst time more than time quantum
					else // if(RR.process[i]->burst_time > RR.time_quantum)
					{
						// ensure process does not exceed time quantum
						if(RR.process[i]->process_tick < (RR.process[i]->burst_time - RR.time_quantum))
						{
							RR.curr_process = i; // end result we want
							RR.process[RR.curr_process]->process_tick ++;
							break;
						}

					}

				}


			}

		}


	}
	else
	{
		// Reset scheduler tick to zero if equal or exceed scheduler total time
		RR.scheduler_tick = 0;

		// Reset processes tick to zero
		for (uint8_t i = 0; i < RR.no_of_process; i++)
		{
			RR.process[i]->process_tick = 0;
		}
	}
}

