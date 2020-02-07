#ifndef BKA_PCB_H
#define BKA_PCB_H

#include "sys.h"
#include "pcb.h"


/**
 * The PCB table, allocating memory for all the available PCBs.
 */
static pcb_t pcb_table[BKA_MAX_PROC];
/**
 * The free PCB table such that
 * 		* free_pcb_table[i] = 0 if process i is NOT free
 * 		* free_pcb_table[i] = 1 otherwise, i.e. process i IS free
 *
 * 	TODO Couldn't we just leverage on a pcb_t inner field for marking free PCBs?
 */
static unsigned free_pcb_table[BKA_MAX_PROC];


/* TODO Implement missing functions. */
void initPcbs(void) {
	int i;

	for (i = 0; i < BKA_MAX_PROC; i++)
		free_pcb_table[i] = 1;
}

void freePcb(pcb_t *p) {
	free_pcb_table[p - pcb_table] = 1;
}

pcb_t* allocPcb(void) {
	int i;
	pcb_t *out = NULL;

	// Cycle either until i is in range or a free PCB is not found
	for (i = 0; i < BKA_MAX_PROC && free_pcb_table[i] == 0; i++);

	// If we found an eligible free PCB, have out point to it and initialize it
	if (i < BKA_MAX_PROC) {
		out = pcb_table + i;

		// TODO Are the other fields properly initialized?
		INIT_LIST_HEAD(&out->next);
		out->parent = NULL;
		INIT_LIST_HEAD(&out->first_child);
		INIT_LIST_HEAD(&out->siblings);
		// TODO Urgent! Properly set out->state
		out->priority = 0;
		out->semkey = NULL;

		free_pcb_table[i] = 0;
	}

	return out;
}


void mkEmptyProcQ(struct list_head *head) {
	INIT_LIST_HEAD(head);
}

int emptyProcQ(struct list_head *head) {
	return head->next == head && head->prev == head;
}

void insertProcQ(struct list_head *head, pcb_t *p) {
	struct list_head *curr_list = head->next;
	// TODO If the list is completely empty, what value is curr_proc going to
	//  assume the first time?
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
