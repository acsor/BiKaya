#include "pcb.h"
#include "string.h"
#include "utils.h"


/**
 * The PCB table, allocating memory for all the available PCBs.
 */
static pcb_t pcb_table[BKA_MAX_PROC];
/**
 * The free PCB list.
 */
static list_t free_pcb_list;


void bka_pcbs_init(void) {
	int i;

	INIT_LIST_HEAD(&free_pcb_list);

	for (i = 0; i < BKA_MAX_PROC; i++)
		list_add_tail(&pcb_table[i].next, &free_pcb_list);
}

pcb_t* bka_pcb_alloc(void) {
	pcb_t *out;
	unsigned i;

	if (list_empty(&free_pcb_list))
		return NULL;

	/* Acquire the PCB from the free PCB list */
	out = container_of(free_pcb_list.next, pcb_t, next);
	list_del_init(&out->next);

	/* Initialize the new PCB */
	INIT_LIST_HEAD(&out->next);
	out->parent = NULL;
	INIT_LIST_HEAD(&out->first_child);
	INIT_LIST_HEAD(&out->siblings);
	/* Initialize the out->state field to all 0s */
	bka_memset(&out->state, 0, sizeof(state_t));
	out->priority = out->original_priority = 0;
	out->semkey = NULL;

	/* Initialize time fields to appropriate values */
	out->timer_curr = out->timers + 0;
	out->timer_prev = out->timers + 1;
	out->timer_bk = bka_time_now();

	for (i = 0; i < BKA_LENGTH(out->timers, time_t); i++)
		out->timers[i] = 0;

	/* Initialize all spec passup areas to NULL */
	out->sp_areas[0][0] = out->sp_areas[0][1] = NULL;
	out->sp_areas[1][0] = out->sp_areas[1][1] = NULL;
	out->sp_areas[2][0] = out->sp_areas[2][1] = NULL;

	return out;
}

void bka_pcb_free(pcb_t *p) {
	list_del_init(&p->next);
	list_add_tail(&p->next, &free_pcb_list);
}

void bka_pcb_init(pcb_t *p, pfun_t f, int original_priority) {
	p->priority = p->original_priority = original_priority;
	bka_memset(&p->state, 0, sizeof(state_t));

#ifdef BKA_ARCH_UMPS
	p->state.pc_epc = (unsigned) f;
	/* Enable interrupts */
	p->state.status |= STATUS_IEp;
	/* Enable interval timer */
	p->state.status |= STATUS_IM(2);
	/* Set stack pointer */
	p->state.reg_sp = BKA_RAMTOP - FRAMESIZE * (bka_pcb_to_pid(p) + 1);
#elif defined(BKA_ARCH_UARM)
	p->state.pc = (unsigned) f;
	/* Enable kernel mode */
	p->state.cpsr  = STATUS_SYS_MODE;
	/* Enable regular interrupt handling and interval timer */
	p->state.cpsr = STATUS_ALL_INT_DISABLE(p->state.cpsr);
	p->state.cpsr = STATUS_ENABLE_TIMER(p->state.cpsr);
	/* Set virtual memory off */
	p->state.CP15_Control = CP15_DISABLE_VM(p->state.CP15_Control);
	/* Set stack pointer */
	p->state.sp = BKA_RAMTOP - FRAMESIZE * (bka_pcb_to_pid(p) + 1);
#endif
}

int bka_pcb_stat(pcb_t const *p) {
	if (p < pcb_table || (pcb_table + BKA_MAX_PROC - 1) < p)
		return BKA_PCB_STAT_INV;
	else if (bka_pcb_queue_contains(&free_pcb_list, p))
		return BKA_PCB_STAT_FREED;

	return 0;
}

int bka_pcb_to_pid (pcb_t const * const p) {
	return p - pcb_table;
}

pcb_t* bka_pid_to_pcb (unsigned pid) {
	return (0 <= pid && pid < BKA_MAX_PROC) ? pcb_table + pid: NULL;
}


void bka_pcb_time_save(pcb_t *p) {
	*p->timer_curr += bka_time_now() - p->timer_bk;
}

void bka_pcb_time_push(pcb_t *p, unsigned type) {
	*p->timer_curr += bka_time_now() - p->timer_bk;
	p->timer_prev = p->timer_curr;

	p->timer_curr = p->timers + type;
	p->timer_bk = bka_time_now();
}

time_t* bka_pcb_time_pop(pcb_t *p) {
	time_t *out = p->timer_curr;

	*p->timer_curr += bka_time_now() - p->timer_bk;
	p->timer_curr = p->timer_prev;
	p->timer_prev = NULL;
	p->timer_bk = bka_time_now();

	return out;
}


void bka_pcb_queue_init(list_t *head) {
	INIT_LIST_HEAD(head);
}

int bka_pcb_queue_isempty(list_t *head) {
	return head->next == head && head->prev == head;
}

void bka_pcb_queue_ins(list_t *head, pcb_t *p) {
	list_t *curr_list = head->next;
	pcb_t *curr_proc = container_of(curr_list, pcb_t, next);

	list_del_init(&p->next);

	/* Iterate until we either end the list or find a matching process with */
	/* priority less than p */
	while (curr_list != head && p->priority < curr_proc->priority) {
		curr_list = curr_list->next;
		curr_proc = container_of(curr_list, pcb_t, next);
	}

	__list_add(&p->next, curr_list->prev, curr_list);
}

pcb_t* bka_pcb_queue_head(list_t *head) {
	return list_empty(head) ? NULL: container_of(head->next, pcb_t, next);
}

int bka_pcb_queue_contains(list_t *head, pcb_t const *p) {
	pcb_t *curr;

	list_for_each_entry(curr, head, next) {
		if (curr == p)
			return 1;
	}

	return 0;
}

pcb_t* bka_pcb_queue_pop(list_t *head) {
	pcb_t *p = NULL;

	if (!list_empty(head)) {
		p = container_of(head->next, pcb_t, next);
		list_del_init(head->next);
	}

	return p;
}

pcb_t* bka_pcb_queue_rm(list_t *head, pcb_t *p) {
	list_t *curr_list = head;
	pcb_t *curr_proc;

	do {
		curr_list = curr_list->next;
		curr_proc = container_of(curr_list, pcb_t, next);
	} while (curr_list != head && curr_proc != p);

	/* If we exited the loop due to finding the corresponding entry */
	if (curr_proc == p) {
		list_del_init(curr_list);

		return p;
	}

	return NULL;
}


int bka_pcb_tree_isempty(pcb_t *this) {
	return list_empty(&this->first_child);
}

void bka_pcb_tree_push(pcb_t *parent, pcb_t *child) {
	list_add_tail(&child->siblings, &parent->first_child);
	child->parent = parent;
}

pcb_t* bka_pcb_tree_pop(pcb_t *p) {
	list_t *to_remove;

	if (list_empty(&p->first_child)) {
		return NULL;
	} else {
		to_remove = p->first_child.next;
		list_del_init(to_remove);
	}

	return container_of(to_remove, pcb_t, siblings);
}

pcb_t* bka_pcb_tree_parentrm(pcb_t *p) {
	if (p->parent) {
		p->parent = NULL;
		list_del_init(&p->siblings);

		return p;
	}

	return NULL;
}
