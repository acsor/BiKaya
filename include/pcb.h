#ifndef BKA_PCB_H
#define BKA_PCB_H

#include "arch.h"
#include "list.h"
#include "time.h"

#define	BKA_PCB_STAT_OK	0
#define	BKA_PCB_STAT_FREED 1
#define	BKA_PCB_STAT_INV 2


/**
 * A process function type, to be associated as the code executed by a given
 * process.
 */
typedef void (*pfun_t)(void);
typedef unsigned int memaddr_t;

/**
 * Process Control Block (PCB) data structure.
 */
typedef struct pcb_t {
		/* Process queue fields */
		list_t next;

		/* Process tree fields */
		struct pcb_t *parent;
		list_t first_child, siblings;

		state_t state;
		int priority, original_priority;

		/* Key of the semaphore which the process is eventually blocked on */
		int *semkey;

		/* A general-purpose timer, used as a counting mechanism for
		 * kernel and user time. */
		time_t timer_bk;
		time_t *timer_prev, *timer_curr;
		/* Cumulative time spent on kernel-level code (timers[0]) and user-level
		 * code (timers[1]).
		 */
		time_t timers[2];

		/* Spec Passup old and new process state areas. */
        state_t *sp_areas[3][2];
} pcb_t;


/* PCB management functions. */
/**
 * Initializes the free PCB list such that it contains all the available
 * processes.
 */
void bka_pcbs_init();
/**
 * @return @c NULL if the free PCB list is empty, a newly-allocated PCB
 * otherwise. The returned element is extracted from the free PCB list.
 */
pcb_t* bka_pcb_alloc();
/**
 * @param p Pointer to a PCB to be inserted into the free PCB list.
 */
void bka_pcb_free(pcb_t *p);
/**
 * Initializes the given @pcb_t process descriptor for running. Specifically,
 * the process status will be set in such a way that
 *
 * <ul>
 * 		<li>Interrupts are enabled</li>
 * 		<li>Virtual memory is off</li>
 * 		<li>The interval timer is enabled, although not started</li>
 * 		<li>Kernel mode is enabled</li>
 * 		<li>The stack pointer equals <tt>RAMTOP - FRAMESIZE * (pid + 1)</tt>,
 * 		where @c pid is the Process ID of the given process (taking values
 * 		in the <tt>0..BKA_MAXPROC - 1</tt> range)</li>
 * 		<li>Priority is set to @c 0</li>
 * 		<li>The process program counter points to @c f address.</li>
 * </ul>
 * @param p @c pcb_t instance to be initialized.
 * @param f Process code @c p will execute.
 * @param original_priority The priority value to be assigned to the @c priority
 * and @c original_priority PCB fields.
 */
void bka_pcb_init(pcb_t *p, pfun_t f, int original_priority);
/**
 * Sets @c p process status to match the state given by @c s.
 */
#define bka_pcb_state_set(p, s)	(p)->state = *(s)
/**
 * @return @c BKA_PCB_STAT_FREED if @c p was returned to the free PCB list, @c
 * BKA_PCB_STAT_INV if it points to an invalid memory area or @c 0 otherwise.
 */
int bka_pcb_stat(pcb_t const *p);
/**
 * @param p A process' PCB to get a PID for. Undefined behavior might occur if
 * @c p is @c NULL or points to an invalid memory area.
 * @return The PID (Process ID) corresponding to the given process control
 * block in the <tt>0..BKA_MAXPROC - 1</tt> range.
 */
int bka_pcb_to_pid(pcb_t const * const p);
/**
 * @return The PCB (Process Control Block) corresponding to the given process
 * ID, or @c NULL if the @c pid argument is out of range.
 */
pcb_t* bka_pid_to_pcb(unsigned pid);


/* PCB time functions. */
void bka_pcb_time_save(pcb_t *p);
void bka_pcb_time_push(pcb_t *p, unsigned type);
time_t* bka_pcb_time_pop(pcb_t *p);


/* PCB queue functions. */
/**
 * Initialize the PCB list pointed to by @c head.
 * @param head Sentinel element of the list
 */
void bka_pcb_queue_init(list_t *head);
/**
 * @return @c true if @c head refers to an empty list, @c false otherwise.
 */
int bka_pcb_queue_isempty(list_t *head);
/**
 * Inserts @c p into the PCB queue pointed to by @c head, so that the
 * process priority is preserved, i.e. the list is ordered in a descending
 * manner with respect to the processes' priority.
 * @param head Pointer to the sentinel of the list to insert to.
 * @param p PCB to be inserted.
 */
void bka_pcb_queue_ins(list_t *head, pcb_t *p);
/**
 * @return The head element of @c list, or @c NULL if that list is empty. No
 * removal is performed.
 */
pcb_t* bka_pcb_queue_head(list_t *head);
/**
 * @return @c 1 if @c head contains @c p, @c 0 otherwise.
 */
int bka_pcb_queue_contains(list_t *head, pcb_t const *p);
/**
 * Removes the first element from the process queue pointed to by @c head.
 * @param head Sentinel element of the list to remove from.
 * @return The first element of the process queue, or @c NULL if the queue
 * contains no elements.
 */
pcb_t* bka_pcb_queue_pop(list_t *head);
/**
 * Removes the PCB pointed to by @c p from the list @c head.
 * @param head List to remove from
 * @param p PCB pointer to remove from @head. @c p might be found at any
 * arbitrary position within @c head.
 * @return @c p if it was successfully removed from @c head, @c NULL
 * otherwise, i.e. @c p was not within @c head.
 */
pcb_t* bka_pcb_queue_rm(list_t *head, pcb_t *p);

/* Tree view functions */
/**
 * @return @c 1 if @c this has no children, @c 0 if it has.
 */
int bka_pcb_tree_isempty(pcb_t *this);
/**
 * Inserts @c child to the end of the children list of @c parent.
 */
void bka_pcb_tree_push(pcb_t *parent, pcb_t *child);
/**
 * @param p PCB node to remove the first child from.
 * @return The first element of the children list of @c p, or @c NULL if @c p
 * has no child nodes in its tree view.
 */
pcb_t* bka_pcb_tree_pop(pcb_t *p);
/**
 * Removes @c p from the children list of its parent.
 *
 * @return @c p itself if it was successfully removed, @c NULL if something
 * goes wrong, e.g. @c p had no parent.
 */
pcb_t* bka_pcb_tree_parentrm(pcb_t *p);


#endif
