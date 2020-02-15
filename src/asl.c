#include "asl.h"
#include "sys.h"
#include "pcb.h"


/**
 * The semaphore table, allocating data for each one of them.
 */
static semd_t semd_table[BKA_MAX_PROC];
/**
 * Head of the Active Semaphore List (ASL).
 */
static struct list_head asl_head;
/**
 * Head of the Free Semaphore List (FSL).
 * Note: this name does not appear in public slides, but belongs only to this
 * project.
 */
static struct list_head fsl_head;


void initASL() {
	int i;

	INIT_LIST_HEAD(&asl_head);
	INIT_LIST_HEAD(&fsl_head);

	// Add all semaphores to the FSL
	for (i = 0; i < BKA_MAX_PROC; i++)
		list_add_tail(&semd_table[i].next, &fsl_head);
}

semd_t* bka_sem_alloc(int *key) {
	semd_t *result;

	if (list_empty(&fsl_head))
		return NULL;

	result = container_of(fsl_head.next, semd_t, next);
	// Remove the newly fetched semaphore from the FSL
	list_del(&result->next);
	result->key = key;
	INIT_LIST_HEAD(&result->proc_queue);
	list_add_tail(&result->next, &asl_head);

	return result;
}

void bka_sem_free(semd_t *s) {
	list_del(&s->next);
	list_add_tail(&s->next, &fsl_head);
}

semd_t* getSemd(int *key) {
	semd_t *s;

	list_for_each_entry(s, &asl_head, next) {
		if (s->key == key)
			return s;
	}

	return NULL;
}

int insertBlocked (int *key, pcb_t *p) {
	// Look for an existing semaphore with the given key
	semd_t *s = getSemd(key);

	// If no such semaphore exists, try to allocate it
	if (!s)
		s = bka_sem_alloc(key);

	// If the semaphore is finally available
	if (s) {
		// TODO Do we need to insert into the queue in a PCB priority order, or
		//  in a FIFO fashion?
		list_add_tail(&p->next, &s->proc_queue);
		p->semkey = key;

		return FALSE;
	} else {
		return TRUE;
	}
}

pcb_t* removeBlocked(int *key) {
	semd_t *s = getSemd(key);
	pcb_t *p = NULL;

	if (s)
		p = removeProcQ(&s->proc_queue);

	if (emptyProcQ(&s->proc_queue))
		bka_sem_free(s);

	return p;
}

pcb_t* outBlocked(pcb_t *p) {
	semd_t *s = getSemd(p->semkey);
	pcb_t *to_remove;

	if (s) {
		list_for_each_entry(to_remove, &s->proc_queue, next) {
			if (to_remove == p) {
				list_del(&to_remove->next);

				if (list_empty(&s->proc_queue))
					bka_sem_free(s);

				return to_remove;
			}
		}
	}

	return NULL;
}

pcb_t* headBlocked(int *key) {
	semd_t *s = getSemd(key);

	return s != NULL ? headProcQ(&s->proc_queue): NULL;
}

void outChildBlocked(pcb_t *p) {
	pcb_t *removed = outBlocked(p);
	pcb_t *child;

	if (removed) {
		list_for_each_entry(child, &removed->first_child, siblings)
			outChildBlocked(child);
	}
}
