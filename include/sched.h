#ifndef BKA_SCHED_H
#define BKA_SCHED_H

#include "list.h"
#include "pcb.h"

/**
 * Scheduling Time Units (STU) constants.
 */
#define BKA_STU_MICRO	1
#define BKA_STU_MILLI	1000
#define BKA_STU_SEC		1000000

/* The Killed Running flag. */
#define BKA_SCHED_KR	1


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
 * Figuratively "kills" the @c to_kill process. In particular, this has the
 * effect to
 *
 * <ul>
 * 	<li>"Kill" all the other descendant processes of @c to_kill</li>
 * 	<li>Remove all killed processes from semaphores they may be pending on/li>
 * 	<li>Return the corresponding PCBs to the free PCB list</li>
 * </ul>
 *
 * @c Note: this function might have the (intended) effect to kill the
 * currently running process. If that's the case, no action whatsoever is
 * performed on @c bka_sched_curr and it is <b>totally up to the caller</b>
 * the decision of what to do next (e.g. perform a context switch).
 *
 * @param to_kill
 * @return @c BKA_E_INVARG if @c to_kill was invalid, BKA_SCHED_KR if the
 * currently running process was killed or just @c 0 otherwise.
 */
int bka_sched_kill(pcb_t *to_kill);
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
