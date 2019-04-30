/**
 * @file timer.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Source file providing a helper function for different tasks to initialize respective timers 
 * @version 0.1
 * @date 2019-03-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "timer.h"
#include "queue.h"


timer_t initTimer(uint64_t nanosec, void (*callbackFunction)() ) {
    timer_t timerid;
	struct itimerspec its;
	struct sigevent sev;
	struct sigaction tsa;

	//Keep a static count of the number of software timers created
	static uint8_t signalCount=0;

	tsa.sa_flags = 0;
	sigemptyset (&tsa.sa_mask);
	tsa.sa_handler=callbackFunction;
	
	//Set the signal action for the corresponding Real-Time signal, provided it does not exceed the limit
	if (SIGRTMIN + signalCount <= SIGRTMAX) {
		sigaction(SIGRTMIN + signalCount, &tsa, NULL);
	}
	else {
		perror("\nUnable to create more timers");
		enQueueForLog(PLAIN_MSG, ERROR, "Unable to create more timers", NULL, NULL);
		return (timer_t)-1;
	}

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN + signalCount;

	if(timer_create(CLOCK_REALTIME, &sev, &timerid)) {
		perror("\ntimer_create");
		fflush(stdout);
		enQueueForLog(PLAIN_MSG, ERROR, "timer_create", NULL, NULL);
		return (timer_t)-1;
	}
	
	its.it_value.tv_sec=nanosec/1000000000;
	its.it_value.tv_nsec=nanosec%1000000000;
	its.it_interval.tv_sec =nanosec/1000000000;
	its.it_interval.tv_nsec = nanosec%1000000000;
	
	if(timer_settime(timerid, 0, &its, NULL)) {
		perror("timer_settime");
		fflush(stdout);
		enQueueForLog(PLAIN_MSG, ERROR, "timer_settime", NULL, NULL);
		return (timer_t)-1;
	}

	++signalCount;

	return timerid;
}