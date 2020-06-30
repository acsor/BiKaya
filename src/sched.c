#include "exc.h"
#include "sched.h"
#include "sem.h"

#define TIME_SLICE		3
#define	TIME_SLICE_UNIT	BK_STU_MILLI


list_t	bk_sched_ready;
pcb_t	*bk_sched_curr;


/**
 * Performs a context switch into the process associated to the @c to_switch
 * PCB. @c to_switch will be removed from the corresponding ready queue, and
 * the current process @c bk_sched_curr reinserted into it.
 *
 * @b Note: it is the caller's responsibility to save the current process
 * state before switching out of it.
 *
 * @param to_switch PCB of the process to switch into. It is the caller's
 * responsibility to check that <tt>to_switch != NULL</tt>.
 * @see bk_sched_switch_top, bk_pbc_state_set
 */
void sched_switch(pcb_t *const to_switch);


void bk_sched_init() {
	bk_pcb_queue_init(&bk_sched_ready);
	bk_sched_curr = NULL;
}

void bk_sched_resume() {
	if (bk_sched_curr) {
		bk_kernel_on_exit();
		bk_sched_it_set(TIME_SLICE, TIME_SLICE_UNIT);

		LDST(&bk_sched_curr->state);
	} else {
		HALT();
	}
}

void bk_sched_enqueue(pcb_t *p) {
	p->priority = p->original_priority;
	bk_pcb_queue_ins(&bk_sched_ready, p);
}

void bk_sched_suspend(pcb_t *to_suspend) {
	if (to_suspend == bk_sched_curr) {
		bk_sched_curr = bk_pcb_queue_pop(&bk_sched_ready);
	} else {
		bk_pcb_queue_rm(&bk_sched_ready, to_suspend);
	}
}

int bk_sched_kill(pcb_t *to_kill) {
	list_t queue;
	/* 1 if we removed the running process from the ready queue, 0 otherwise. */
	int killed_running = bk_sched_curr == to_kill;

	if (bk_pcb_stat(to_kill))
		return BK_E_INVARG;

	INIT_LIST_HEAD(&queue);
	list_del_init(&to_kill->next);
	list_add_tail(&to_kill->next, &queue);

	/* Perform a level-wide scanning of the process tree rooted in to_kill. */
	while (!list_empty(&queue)) {
		pcb_t *curr = bk_pcb_queue_pop(&queue);
		pcb_t *child;

		killed_running |= bk_sched_curr == curr;

		/* Remove each child from the ready queue (if at all there) and
		 * insert into @c queue for a recursive examination. */
		list_for_each_entry(child, &curr->first_child, siblings) {
			list_del_init(&child->next);
			list_add_tail(&child->next, &queue);
		}

		/* Finally free up the PCB: remove it from the semaphore queue it is
		 * pending on (if need to), from its parent and put it back into the
		 * FPL (Free PCB List). */
		if (curr->semkey) {
			(*curr->semkey)++;
			bk_sem_pcb_rm(curr);
		}

		bk_pcb_tree_parentrm(curr);
		bk_pcb_free(curr);
	}

	if (killed_running) {
		bk_sched_curr = bk_pcb_queue_pop(&bk_sched_ready);
		bk_sched_curr->timer_bk = bk_time_now();

		return BK_SCHED_KR;
	} else {
		return 0;
	}
}

void bk_sched_switch_top() {
	if (!list_empty(&bk_sched_ready))
		sched_switch(bk_pcb_queue_head(&bk_sched_ready));
	else if (bk_sched_curr)
		bk_sched_resume();
	else
		HALT();
}


void bk_sched_it_set(unsigned time, unsigned unit) {
	time_t *interval_timer = (unsigned*) BUS_REG_TIMER;
	time_t const time_scale = *((unsigned*) BUS_REG_TIME_SCALE);

	*interval_timer = time * time_scale * unit;
}


void sched_switch(pcb_t *const to_switch) {
	pcb_t *queued_pcb;

	/* Update priorities of old processes to avoid starvation. */
	list_for_each_entry(queued_pcb, &bk_sched_ready, next)
		queued_pcb->priority++;

	/* Reset the current process priority, reinsert it into the ready queue
	 * and update its kernel time.  If bk_sched_curr == NULL, this might be
	 * the first run or one in which the queue was depleted.
	 */
	if (bk_sched_curr) {
		bk_sched_curr->priority = bk_sched_curr->original_priority;
		bk_pcb_queue_ins(&bk_sched_ready, bk_sched_curr);
		bk_pcb_time_save(bk_sched_curr);
	}

	/* Renew the running process and extract if from the queue. */
	bk_sched_curr = to_switch;
	/* bk_shed_curr now points to a PCB just extracted form bk_sched_ready,
	 * but its time breakpoint timer_bk is set to an old value. */
	bk_sched_curr->timer_bk = bk_time_now();
	list_del_init(&bk_sched_curr->next);

	bk_sched_resume();
}
