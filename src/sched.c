#include "sched.h"
#include "string.h"
#include "sys.h"


list_t	bka_sched_ready;
pcb_t	*bka_sched_curr;


void bka_sched_init() {
	bka_pcb_queue_init(&bka_sched_ready);
	bka_sched_curr = NULL;
}

void bka_sched_ready_enqueue(pcb_t *p) {
	bka_pcb_queue_ins(&bka_sched_ready, p);
}

void bka_sched_switch_top_hard() {
	/* TODO The running process might have some children to it. What to do in
	 * case of an hard switch? */
	if (bka_sched_curr)
		bka_pcb_free(bka_sched_curr);

	if (list_empty(&bka_sched_ready)) {
		bka_sched_curr = NULL;
		HALT();
	} else {
		bka_sched_curr = bka_pcb_queue_pop(&bka_sched_ready);

		bka_sched_it_set(TIME_SLICE, TIME_SLICE_UNIT);
		LDST(&bka_sched_curr->state);
	}
}

void bka_sched_switch_top(state_t *curr_proc_status) {
	if (list_empty(&bka_sched_ready)) {
		bka_sched_switch(bka_sched_curr, curr_proc_status);
	} else {
		bka_sched_switch(bka_pcb_queue_head(&bka_sched_ready), curr_proc_status);
	}
}

void bka_sched_switch(pcb_t *const to_switch, state_t *curr_proc_status) {
	pcb_t *queued_pcb;

	/* Update priorities of old processes to avoid starvation. */
	list_for_each_entry(queued_pcb, &bka_sched_ready, next)
		queued_pcb->priority++;

	/* Store the current process status, reset its priority and set it into
	 * the ready process queue. */
	bka_memcpy(&bka_sched_curr->state, curr_proc_status, sizeof(state_t));
	bka_sched_curr->priority = bka_sched_curr->original_priority;
	bka_pcb_queue_ins(&bka_sched_ready, bka_sched_curr);

	/* Renew the running process and extract if from the queue. */
	bka_sched_curr = to_switch;
	bka_pcb_queue_rm(&bka_sched_ready, to_switch);

	bka_sched_it_set(TIME_SLICE, TIME_SLICE_UNIT);
	LDST(&to_switch->state);
}


void bka_sched_it_set(unsigned time, unsigned unit) {
	unsigned *interval_timer = (unsigned*) BUS_REG_TIMER;
	unsigned const time_scale = *((unsigned*) BUS_REG_TIME_SCALE);

	*interval_timer = time * time_scale * unit;
}
