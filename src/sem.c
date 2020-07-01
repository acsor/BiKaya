#include "arch.h"
#include "io.h"
#include "pcb.h"
#include "sched.h"
#include "sem.h"

/**
 * The semaphore table, allocating data for each one of them.
 */
static semd_t semd_table[BK_MAX_SEM];
/**
 * Head of the Active Semaphore List (ASL).
 */
static list_t sem_head;
/**
 * Head of the Free Semaphore List (FSL).
 * Note: this name does not appear in public slides, but belongs only to this
 * project.
 */
static list_t fsl_head;


void bk_sem_init() {
	unsigned i;

	INIT_LIST_HEAD(&sem_head);
	INIT_LIST_HEAD(&fsl_head);

	/* Add all semaphores to the FSL */
	for (i = 0; i < BK_MAX_SEM; i++)
		list_add_tail(&semd_table[i].next, &fsl_head);
}

semd_t* bk_sem_alloc(int *key) {
	semd_t *result;

	if (list_empty(&fsl_head))
		return NULL;

	result = container_of(fsl_head.next, semd_t, next);
	/* Remove the newly fetched semaphore from the FSL */
	list_del_init(&result->next);
	result->key = key;
	INIT_LIST_HEAD(&result->proc_queue);
	list_add_tail(&result->next, &sem_head);

	return result;
}

int bk_sem_p(int *semkey, pcb_t *p) {
	(*semkey)--;

	if (*semkey < 0) {
		bk_sched_suspend(p);
		bk_sem_enqueue(semkey, p);
	}

	return *semkey;
}

pcb_t* bk_sem_v(int *semkey) {
	(*semkey)++;

	if (*semkey <= 0) {
		pcb_t *to_restore = bk_sem_dequeue(semkey);

		if (to_restore) {
			to_restore->semkey = NULL;
			bk_sched_enqueue(to_restore);
		} else {
			bk_term_puts2(1, "bk_sem_v(): warning: process removal with an empty queue\n", NULL);
		}

		return to_restore;
	}

	return NULL;
}

void bk_sem_free(semd_t *s) {
	list_del_init(&s->next);
	list_add_tail(&s->next, &fsl_head);
}

semd_t* bk_sem_get(int *key) {
	semd_t *s;

	list_for_each_entry(s, &sem_head, next) {
		if (s->key == key)
			return s;
	}

	return NULL;
}

int bk_sem_enqueue (int *key, pcb_t *p) {
	/* Look for an existing semaphore descriptor with the given key */
	semd_t *s = bk_sem_get(key);

	/* If no such semaphore data structure exists, try to allocate it */
	if (!s)
		s = bk_sem_alloc(key);

	/* If the semaphore descriptor is finally available */
	if (s) {
		list_add_tail(&p->next, &s->proc_queue);
		p->semkey = key;

		return FALSE;
	} else {
		return TRUE;
	}
}

pcb_t* bk_sem_dequeue(int *key) {
	semd_t *s = bk_sem_get(key);
	pcb_t *p = NULL;

	if (s) {
		p = bk_pcb_queue_pop(&s->proc_queue);

		if (bk_pcb_queue_isempty(&s->proc_queue))
			bk_sem_free(s);
	}

	return p;
}

pcb_t* bk_sem_head(int *key) {
	semd_t *s = bk_sem_get(key);

	return s != NULL ? bk_pcb_queue_head(&s->proc_queue) : NULL;
}

pcb_t* bk_sem_pcb_rm(pcb_t *p) {
	semd_t *s = bk_sem_get(p->semkey);
	pcb_t *to_remove;

	if (s) {
		list_for_each_entry(to_remove, &s->proc_queue, next) {
			if (to_remove == p) {
				list_del_init(&to_remove->next);

				if (list_empty(&s->proc_queue))
					bk_sem_free(s);

				return to_remove;
			}
		}
	}

	return NULL;
}

void bk_sem_pcb_rm_desc(pcb_t *p) {
	pcb_t *removed = bk_sem_pcb_rm(p);
	pcb_t *child;

	if (removed) {
		list_for_each_entry(child, &removed->first_child, siblings) bk_sem_pcb_rm_desc(
					child);
	}
}
