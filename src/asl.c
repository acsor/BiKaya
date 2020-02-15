#include "asl.h"
#include "sys.h"
#include "pcb.h"


/**
 * The semaphore table.
 */
static semd_t semd_table[BKA_MAX_PROC];

/**
 * Free semaphore table such that
 * 		* free_semd_table[i] = 0 if semaphore i NOT free
 * 		* free_semd_table[i] = 1 if semaphore i free
 */
static unsigned free_semd_table[BKA_MAX_PROC];


void initASL() {
	int i;

	for (i = 0; i < BKA_MAX_PROC; i++)
		free_semd_table[i] = 1;
}

void bka_sem_free(semd_t *s) {
	free_semd_table[s - semd_table] = 1;
}

semd_t* getSemd(int *key) {
	semd_t *result = NULL;
	int i;

	for (i = 0; i < BKA_MAX_PROC; i++) {
		if (*semd_table[i].key == *key && free_semd_table[i] == 0)
			break;
	}

	if (i < BKA_MAX_PROC)
		result = semd_table + i;

	return result;
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
