#ifndef BK_SCHED_H
#define BK_SCHED_H

#include "arch.h"
#include "list.h"
#include "pcb.h"

/**
 * Scheduling Time Units (STU) constants.
 */
#define BK_STU_MICRO	1
#define BK_STU_MILLI	1000
#define BK_STU_SEC		1000000

/**
 * The Killed Running flag.
 * @see bk_sched_kill()
 */
#define BK_SCHED_KR	1


/**
 * The scheduler ready queue.
 */
extern list_t	bk_sched_ready;
/**
 * Currently running PCB.
 */
extern pcb_t	*bk_sched_curr;


/**
 * Initializes variables for the correct functioning of the scheduler module.
 * To be called once and only once per program execution before starting to
 * access this module.
 */
void bk_sched_init();
/**
 * Resumes the currently running process with a renewed time slice. No
 * changes are performed on the ready queue.
 */
void bk_sched_resume();
/**
 * Enqueues @c p into the ready process queue according to its process
 * priority. Undefined behavior might occur if this function is called
 * consecutively twice (or more) with the same argument.
 * @param p PCB corresponding to the enqueued process.
 */
void bk_sched_enqueue(pcb_t *p);
/**
 * Deletes the topmost process from @c bk_sched_ready returning it to the
 * caller, while performing additional operations such as time updating.
 * @return The most prioritized process, or @c NULL if the ready queue
 * bk_sched_ready is empty.
 */
pcb_t* bk_sched_dequeue();
/**
 * @param to_suspend
 */
void bk_sched_suspend(pcb_t *to_suspend);
/**
 * Figuratively "kills" the @c to_kill process. In particular, this has the
 * effect to
 *
 * <ul>
 * 	<li>"Kill" all the other descendant processes of @c to_kill</li>
 * 	<li>Remove all killed processes from semaphores they may be pending on</li>
 * 	<li>Return the corresponding PCBs to the free PCB list</li>
 * </ul>
 *
 * @c Note: this function might have the (intended) effect to kill the
 * currently running process. When that happens, @c bk_sched_curr is
 * assigned the next process from the ready queue (or @c NULL if none is
 * available). It will be the caller's responsibility to perform a context
 * switch.
 *
 * @param to_kill
 * @return @c BK_E_INVARG if @c to_kill was invalid, BK_SCHED_KR if the
 * currently running process was killed or @c 0 otherwise.
 */
int bk_sched_kill(pcb_t *to_kill);
/**
 * Performs a context switch into the most prioritized process, reinserting
 * the currently running process @c bk_sched_curr into the ready queue @c
 * bk_sched_ready. Leads the system to an @c HALT() state if @c
 * bk_sched_ready is empty and <tt>bk_sched_curr == NULL</tt>.
 */
void bk_sched_switch_top();

/**
 * Sets the system-wide interval timer.
 * @param time Time (in the given unit) to elapse before the interval timer
 * interrupt.
 * @param unit Time unit, to be chosen among @c BK_STU_MICRO, @c
 * BK_STU_MILLI or @c BK_STU_SEC.
 */
void bk_sched_it_set(unsigned time, unsigned unit);


#endif
