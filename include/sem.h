#ifndef BKA_SEM_H
#define BKA_SEM_H

#include "list.h"
#include "pcb.h"

/**
 * The maximum number of allowable @c semd_t (semaphore descriptor) variables.
 * Note that this does not restrict the amount of semaphores definable (which
 * take the @c int* type) for a @ c semd_t is used only when at least a
 * process is pending on a given semaphore; since the number of processes
 * cannot exceed @c BKA_MAX_PROC, so too does the number of @c semd_t variables.
 */
#define	BKA_MAX_SEM	BKA_MAX_PROC

/**
 * Semaphore Descriptor (SEMD) data structure.
 */
typedef struct semd_t {
		list_t next;
		/* Queue of PCBs blocked on the semaphore */
		list_t proc_queue;

		/* Semaphore key */
		int *key;
} semd_t;


/**
 * Initializes the Active and Free Semaphore Lists (ASL and FSL). To be
 * called before issuing any other call to semaphore functions.
 */
void bka_sem_init();
/**
 * @return A newly allocated semaphore whose key is initialized to @c key, or
 * @c NULL if the FSL is empty.
 */
semd_t* bka_sem_alloc(int *key);
int bka_sem_p(int *semkey, pcb_t *p);
/**
 * Performs a V() operation on the semaphore @c s.
 * @param semkey Semaphore on which to perform a Verhogen operation.
 * @return PCB that was eventually reinserted into the ready queue, @c NULL
 * if no other process was waiting on @c s.
 */
pcb_t* bka_sem_v(int *semkey);
/**
 * Frees up the semaphore data structure pointed to by @c s.
 * @param s Pointer to the semaphore data structure to be freed.
 */
void bka_sem_free(semd_t *s);
/**
 * @return A pointer to the semaphore in the ASL whose key equals @c key, or @c
 * NULL if no such element exists.
 */
semd_t* bka_sem_get(int *key);
/**
 * Inserts @c p to the queue of the semaphore whose key equals @c key
 * @param key Key of the semaphore in which to insert @c p. If no semaphore
 * with such key exists, a new one, if possible, is allocated.
 * @param p PCB to enqueue (insert as last element) into the queue
 * @return @c TRUE if the operation fails due to the free semaphore list
 * being empty (and @c p not being able to be inserted into the identified
 * queue), @c FALSE otherwise.
 */
int bka_sem_enqueue(int *key, pcb_t* p);
/**
 * Removes the first element of the PCB queue associated to the semaphore whose
 * key matches @c key. If the PCB queue is emptied, the associated semaphore
 * data structure is returned to the free list.
 * @param key Key of the semaphore from which to perform the removal.
 * @return A pointer to the removed PCB element, or @c NULL if either no
 * semaphore with the given key exists or if its PCB queue is empty.
 */
pcb_t* bka_sem_dequeue(int *key);
/**
 * @return A pointer to the first element of the PCB queue associated to the
 * semaphore whose key equals @c key, or @c NULL if either no semaphore with the
 * given key exists or if its PCB queue is empty.
 */
pcb_t* bka_sem_head(int *key);
/**
 * Removes @c p from the semaphore queue to which it belongs. Upon freeing the
 * PCB queue completely, the semaphore to which @p belonged is returned to
 * the free semaphore list.
 * @return @c p itself if it was successfully removed from the semaphore
 * list, @c NULL if there was no such association.
 */
pcb_t* bka_sem_pcb_rm(pcb_t *p);
/**
 * Removes @c p from the semaphore queue to which it belongs. Furthermore, all
 * PCBs located in the PCB tree having as root @c p are also removed from their
 * associated sempahore queues, if such associations exist.
 */
void bka_sem_pcb_rm_desc(pcb_t *p);


#endif
