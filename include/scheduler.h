#ifndef BKA_SCHEDULER_H
#define BKA_SCHEDULER_H

#include "list.h"

/**
 * Scheduling Time Units (STU) constants.
 */
#define BKA_STU_MICRO	1
#define BKA_STU_MILLI	1000
#define BKA_STU_SEC		1000000

#define TIME_SLICE		3
#define	TIME_SLICE_UNIT	BKA_STU_MILLI

/**
 * The scheduler ready queue.
 */
extern list_t	bka_sched_ready;
/**
 * Currently running PCB.
 */
extern pcb_t	*bka_sched_curr;


/**
 * Initializes variables for the correct functioning of the scheduler module.
 */
void bka_sched_init();
/**
 * Enqueues @c p into the ready process queue according to its process
 * priority. Undefined behavior might occur if this function is called
 * consecutively twice (or more) with the same argument.
 * @param p PCB corresponding to the enqueued process.
 */
void bka_sched_ready_enqueue(pcb_t *p);
/**
 * Like @c bka_sched_switch_top(), but performs no state backup of the
 * running process and does not reinsert it into the ready queue (nor does it
 * actually presume the presence of a running process). Hence, the running
 * process is effectively discarded.
 *
 * This function is ideal when booting up the very first process or switching
 * to a new process disregarding the running one (e.g. killing the current
 * process and switching to the other).
 *
 * The function behavior upon a call with one remaining process only is to halt
 * the processor.
 * @see bka_sched_switch, bka_sched_switch_top, bka_sys_kill
 */
void bka_sched_switch_top_hard();
/**
 * A wrapper for @c bka_sched_switch(), selecting the most prioritized
 * process as the one to switch into.
 * @param curr_proc_status Memory area where the current process status is
 * stored.
 * @see bka_sched_switch
 */
void bka_sched_switch_top(state_t *curr_proc_status);
/**
 * Performs a context switch into the process associated to the @c to_switch
 * PCB. @c to_switch will be removed from the corresponding ready queue, and
 * the current process reinserted into it.
 * @param to_switch PCB of the process to switch into.
 * @param curr_proc_status Memory area where the current process status is
 * stored.
 */
void bka_sched_switch(pcb_t *const to_switch, state_t *curr_proc_status);

/**
 * Sets the system-wide interval timer.
 * @param time Time (in the given unit) to elapse before the interval timer
 * interrupt.
 * @param unit Time unit, to be chosen among @c BKA_STU_MICRO, @c
 * BKA_STU_MILLI or @c BKA_STU_SEC.
 */
void bka_sched_it_set(unsigned time, unsigned unit);


#endif
