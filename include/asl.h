#ifndef BKA_ASL_H
#define BKA_ASL_H

#include "list.h"
#include "pcb.h"


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
void initASL();
/**
 * @return A newly allocated semaphore whose key is initialized to @c key, or
 * @c NULL if the FSL is empty.
 */
semd_t* bka_sem_alloc(int *key);
/**
 * @param s Pointer to a semaphore to return to the free list.
 */
void bka_sem_free(semd_t *s);
/**
 * @return A pointer to the semaphore in the ASL whose key equals @c key, or @c
 * NULL if no such element exists.
 */
semd_t* getSemd(int *key);
/**
 * Inserts @c p to the queue of the semaphore whose key equals @c key
 * @param key Key of the semaphore in which to insert @c p. If no semaphore
 * with such key exists, a new one, if possible, is allocated.
 * @param p PCB to insert into the queue
 * @return @c TRUE if an attempt to allocate a new semaphore while the free
 * semaphore list was empty was made, @c FALSE otherwise.
 */
int insertBlocked(int *key, pcb_t* p);
/**
 * Removes the first element of the PCB queue associated to the semaphore whose
 * key matches @c key. If the PCB queue is emptied, the associated semaphore
 * is returned to the free list.
 * @param key Key of the semaphore from which to perform the removal.
 * @return A pointer to the removed PCB element, or @c NULL if either no
 * semaphore with the given key exists or if its PCB queue is empty.
 */
pcb_t* removeBlocked(int *key);
/**
 * Removes @c p from the semaphore queue to which it belongs. Upon freeing the
 * PCB queue completely, the semaphore to which @p belonged is returned to
 * the free semaphore list.
 * @return @c p itself if it was successfully removed from the semaphore
 * list, @c NULL if there was no such association.
 */
pcb_t* outBlocked(pcb_t *p);
/**
 * @return A pointer to the first element of the PCB queue associated to the
 * semaphore whose key equals @c key, or @c NULL if either no semaphore with the
 * given key exists or if its PCB queue is empty.
 */
pcb_t* headBlocked(int *key);
/**
 * Removes @c p from the semaphore queue to which it belongs. Furthermore, all
 * PCBs located in the PCB tree having as root @c p are also removed from their
 * associated sempahore queues, if such associations exist.
 */
void outChildBlocked(pcb_t *p);


#endif
