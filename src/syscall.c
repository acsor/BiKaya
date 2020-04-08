#include "pcb.h"
#include "scheduler.h"
#include "syscall.h"


void bka_sys_call(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3) {
	/* TODO Until phase 1.5 continues, I see no reason to properly dispatch
	 *  to different syscalls. When specifications will be clearer, I'll
	 *  define this function extensively */
	if (id == BKA_SYS_KILL)
		bka_sys_kill_running();
	else
		PANIC();
}

void bka_sys_kill_running() {
	if (bka_sched_curr)
		bka_sys_kill(bka_pcb_to_pid(bka_sched_curr));
}

void bka_sys_kill(unsigned pid) {
	pcb_t *to_kill = bka_pid_to_pcb(pid);
	list_t queue;
	/* 1 if we removed the running process from the ready queue, 0 otherwise. */
	int killed_running = bka_sched_curr == to_kill;

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
	}

	/* If, among the others, we killed the running process, we have to properly
	 * switch to another process. */
	if (killed_running)
		bka_sched_switch_top_hard();
}
