#include "asl.h"
#include "arch.h"
#include "pcb.h"


/**
 * The semaphore table, allocating data for each one of them.
 */
static semd_t semd_table[BKA_MAX_PROC];
/**
 * Head of the Active Semaphore List (ASL).
 */
static list_t asl_head;
/**
 * Head of the Free Semaphore List (FSL).
 * Note: this name does not appear in public slides, but belongs only to this
 * project.
 */
static list_t fsl_head;


void bka_sem_init() {
	int i;

	INIT_LIST_HEAD(&asl_head);
	INIT_LIST_HEAD(&fsl_head);

	/* Add all semaphores to the FSL */
	for (i = 0; i < BKA_MAX_PROC; i++)
		list_add_tail(&semd_table[i].next, &fsl_head);
}

semd_t* bka_sem_alloc(int *key) {
	semd_t *result;

	if (list_empty(&fsl_head))
		return NULL;

	result = container_of(fsl_head.next, semd_t, next);
	/* Remove the newly fetched semaphore from the FSL */
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

semd_t* bka_sem_get(int *key) {
	semd_t *s;

	list_for_each_entry(s, &asl_head, next) {
		if (s->key == key)
			return s;
	}

	return NULL;
}

int bka_sem_enqueue (int *key, pcb_t *p) {
	/* Look for an existing semaphore with the given key */
	semd_t *s = bka_sem_get(key);

	/* If no such semaphore exists, try to allocate it */
	if (!s)
		s = bka_sem_alloc(key);

	/* If the semaphore is finally available */
	if (s) {
		list_add_tail(&p->next, &s->proc_queue);
		p->semkey = key;

		return FALSE;
	} else {
		return TRUE;
	}
}

pcb_t* bka_sem_dequeue(int *key) {
	semd_t *s = bka_sem_get(key);
	pcb_t *p = NULL;

	if (s)
		p = bka_pcb_queue_pop(&s->proc_queue);

	if (bka_pcb_queue_isempty(&s->proc_queue))
		bka_sem_free(s);

	return p;
}

pcb_t* bka_sem_head(int *key) {
	semd_t *s = bka_sem_get(key);

	return s != NULL ? bka_pcb_queue_head(&s->proc_queue) : NULL;
}

pcb_t* bka_sem_pcb_rm(pcb_t *p) {
	semd_t *s = bka_sem_get(p->semkey);
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

void bka_sem_pcb_rm_desc(pcb_t *p) {
	pcb_t *removed = bka_sem_pcb_rm(p);
	pcb_t *child;

	if (removed) {
		list_for_each_entry(child, &removed->first_child, siblings) bka_sem_pcb_rm_desc(
					child);
	}
}
