#ifndef BKA_PCB_H
#define BKA_PCB_H

#include "string.h"
#include "sys.h"
#include "pcb.h"

#ifdef BKA_ARCH_UMPS
#include "umps/cp0.h"
#endif


/**
 * The PCB table, allocating memory for all the available PCBs.
 */
static pcb_t pcb_table[BKA_MAX_PROC];
/**
 * The free PCB list.
 */
static struct list_head free_pcb_list;


void initPcbs(void) {
	int i;

	INIT_LIST_HEAD(&free_pcb_list);

	for (i = 0; i < BKA_MAX_PROC; i++)
		list_add_tail(&pcb_table[i].next, &free_pcb_list);
}

void freePcb(pcb_t *p) {
	list_add_tail(&p->next, &free_pcb_list);
}

pcb_t* allocPcb(void) {
	pcb_t *out;

	if (list_empty(&free_pcb_list))
		return NULL;

	// Acquire the PCB from the free PCB list
	out = container_of(free_pcb_list.next, pcb_t, next);
	list_del(&out->next);

	// Initialize the new PCB
	INIT_LIST_HEAD(&out->next);
	out->parent = NULL;
	INIT_LIST_HEAD(&out->first_child);
	INIT_LIST_HEAD(&out->siblings);
	// Initialize the out->state field to all 0s
	bka_memset(&out->state, 0, sizeof(state_t));
	out->priority = 0;
	out->original_priority = 0;
	out->semkey = NULL;

	return out;
}


void bka_pcb_init(pcb_t *p, pfun_t f, int original_priority) {
	p->priority = p->original_priority = original_priority;
	bka_memset(&p->state, 0, sizeof(p->state));

#ifdef BKA_ARCH_UMPS
	p->state.pc_epc = (unsigned) f;
	// Enable interrupts
	p->state.status |= STATUS_IEp | STATUS_IEc;
	// Enable interval timer
	p->state.status |= STATUS_IM(2);
	// Set stack pointer
	p->state.reg_sp = BKA_RAMTOP - FRAMESIZE * bka_pcb_to_pid(p);
#elif defined(BKA_ARCH_UARM)
	// Virtual memory off because the corresponding bit is set to 0
	p->state.pc = (unsigned) f;
	// Enable kernel mode
	p->state.cpsr |= STATUS_SYS_MODE;
	// Enable regular interrupt handling
	// TODO Is it correct to enable the interrupt handling mode by unsetting IRQ and FIQ?
	// set stack pointer
	p->state.sp = BKA_RAMTOP - FRAMESIZE * bka_pcb_to_pid(p);
#endif
}

int bka_pcb_to_pid (pcb_t const * const p) {
	return p - pcb_table;
}

pcb_t* bka_pid_to_pcb (unsigned pid) {
	return (0 <= pid && pid < BKA_MAX_PROC) ? pcb_table + pid: NULL;
}


void mkEmptyProcQ(struct list_head *head) {
	INIT_LIST_HEAD(head);
}

int emptyProcQ(struct list_head *head) {
	return head->next == head && head->prev == head;
}

void insertProcQ(struct list_head *head, pcb_t *p) {
	struct list_head *curr_list = head->next;
	pcb_t *curr_proc = container_of(curr_list, pcb_t, next);

	// Iterate until we either end the list or find a matching process with
	// priority less than p
	while (curr_list != head && p->priority < curr_proc->priority) {
		curr_list = curr_list->next;
		curr_proc = container_of(curr_list, pcb_t, next);
	}

	__list_add(&p->next, curr_list->prev, curr_list);
}

pcb_t* headProcQ(struct list_head *head) {
	return list_empty(head) ? NULL: container_of(head->next, pcb_t, next);
}

pcb_t* removeProcQ(struct list_head *head) {
	pcb_t *p = NULL;

	if (!list_empty(head)) {
		p = container_of(head->next, pcb_t, next);
		list_del(head->next);
	}

	return p;
}

pcb_t* outProcQ(struct list_head *head, pcb_t *p) {
	struct list_head *curr_list = head;
	pcb_t *curr_proc;

	do {
		curr_list = curr_list->next;
		curr_proc = container_of(curr_list, pcb_t, next);
	} while (curr_list != head && curr_proc != p);

	// If we exited the loop due to finding the corresponding entry
	if (curr_proc == p) {
		list_del(curr_list);

		return p;
	}

	return NULL;
}


int emptyChild(pcb_t *this) {
	return list_empty(&this->first_child);
}

void insertChild(pcb_t *parent, pcb_t *child) {
	list_add_tail(&child->siblings, &parent->first_child);
	child->parent = parent;
}

pcb_t* removeChild(pcb_t *p) {
	struct list_head *to_remove;

	if (list_empty(&p->first_child)) {
		return NULL;
	} else {
		to_remove = p->first_child.next;
		list_del(to_remove);
	}

	return container_of(to_remove, pcb_t, siblings);
}

pcb_t* outChild(pcb_t *p) {
	struct list_head *to_remove = NULL;
	pcb_t *curr_proc = NULL;

	if (p->parent != NULL) {
		list_for_each(to_remove, &p->parent->first_child) {
			curr_proc = container_of(to_remove, pcb_t, siblings);

			if (curr_proc == p) {
				list_del(to_remove);

				return p;
			}
		}
	}

	return NULL;
}


#endif
