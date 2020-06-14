#include "exc.h"
#include "sched.h"
#include "arch.h"

#define TIME_SLICE		3
#define	TIME_SLICE_UNIT	BKA_STU_MILLI


list_t	bka_sched_ready;
pcb_t	*bka_sched_curr;


void bka_sched_init() {
	bka_pcb_queue_init(&bka_sched_ready);
	bka_sched_curr = NULL;
}

void bka_sched_resume() {
	if (bka_sched_curr) {
		bka_kernel_on_exit();
		bka_sched_it_set(TIME_SLICE, TIME_SLICE_UNIT);

		LDST(&bka_sched_curr->state);
	} else {
		HALT();
	}
}

void bka_sched_enqueue(pcb_t *p) {
	bka_pcb_queue_ins(&bka_sched_ready, p);
}

void bka_sched_suspend(pcb_t *to_suspend) {
	if (to_suspend == bka_sched_curr) {
		bka_sched_curr = bka_pcb_queue_pop(&bka_sched_ready);
	} else {
		bka_pcb_queue_rm(&bka_sched_ready, to_suspend);
	}
}

int bka_sched_kill(pcb_t *to_kill) {
	list_t queue;
	/* 1 if we removed the running process from the ready queue, 0 otherwise. */
	int killed_running = bka_sched_curr == to_kill;

	if (bka_pcb_stat(to_kill))
		return BKA_E_INVARG;

	/* TODO Important! Remove processes from semaphores they wait on. */
	INIT_LIST_HEAD(&queue);
	bka_pcb_queue_rm(&bka_sched_ready, to_kill);
	list_add_tail(&to_kill->next, &queue);

	/* Perform a level-wide scanning of the process tree rooted in to_kill. */
	while (!list_empty(&queue)) {
		pcb_t *curr = bka_pcb_queue_pop(&queue);
		pcb_t *child;

		killed_running |= bka_sched_curr == curr;

		/* Remove each child from the ready queue (if at all there) and
		 * insert into @c queue for a recursive examination. */
		list_for_each_entry(child, &curr->first_child, siblings) {
			bka_pcb_queue_rm(&bka_sched_ready, child);
			list_add_tail(&child->next, &queue);
		}

		/* Finally free up the PCB. */
		bka_pcb_free(curr);
	}

	if (killed_running) {
		bka_sched_curr = bka_pcb_queue_pop(&bka_sched_ready);

		return BKA_SCHED_KR;
	} else {
		return 0;
	}
}

void bka_sched_switch_top() {
	if (list_empty(&bka_sched_ready)) {
		bka_sched_switch(bka_sched_curr);
	} else {
		bka_sched_switch(bka_pcb_queue_head(&bka_sched_ready));
	}
}

void bka_sched_switch(pcb_t *const to_switch) {
	pcb_t *queued_pcb;

	/* Update priorities of old processes to avoid starvation. */
	list_for_each_entry(queued_pcb, &bka_sched_ready, next)
		queued_pcb->priority++;

	/* Reset the current process priority and reinsert it into the ready queue.
	 * If bka_sched_curr == NULL, this might be the first run or one in which
	 * the queue was depleted.
	 */
	if (bka_sched_curr) {
		bka_sched_curr->priority = bka_sched_curr->original_priority;
		bka_pcb_queue_ins(&bka_sched_ready, bka_sched_curr);
	}

	/* Renew the running process and extract if from the queue. */
	bka_sched_curr = to_switch;
	bka_pcb_queue_rm(&bka_sched_ready, to_switch);

	bka_kernel_on_exit();
	bka_sched_it_set(TIME_SLICE, TIME_SLICE_UNIT);

	LDST(&to_switch->state);
}


void bka_sched_it_set(unsigned time, unsigned unit) {
	unsigned *interval_timer = (unsigned*) BUS_REG_TIMER;
	unsigned const time_scale = *((unsigned*) BUS_REG_TIME_SCALE);

	*interval_timer = time * time_scale * unit;
}
