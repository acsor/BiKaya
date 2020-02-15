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

}

void bka_sem_free(semd_t *s) {
	list_del(&s->next);
	list_add_tail(&s->next, &fsl_head);
}

semd_t* getSemd(int *key) {
	struct list_head *curr;
	semd_t *s;

	list_for_each(curr, &asl_head) {
		s = container_of(curr, semd_t, next);

		if (*s->key == *key)
			return s;
	}

	return NULL;
}

/* TODO Implement */
int insertBlocked (int *key,pcb_t* p) {
	return 0;
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

/* TODO Implement */
pcb_t* outBlocked(pcb_t *p) {
	return NULL;
}

pcb_t* headBlocked(int *key) {
	semd_t *s = getSemd(key);

	return s != NULL ? headProcQ(&s->proc_queue): NULL;
}

/* TODO Implement. */
void outChildBlocked(pcb_t *p) {

}
