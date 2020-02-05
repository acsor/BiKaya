#ifndef PCB_H
#define PCB_H

#include "types.h"


/* Free PCB list handling functions. */
/**
 * Initializes the free PCB list such that it contains all the available
 * processes.
 */
void initPcbs();
/**
 * @param p Pointer to a PCB to be inserted into the free PCB list.
 */
void freePcb(pcb_t *p);
/**
 * @return @c NULL if the free PCB list is empty, a newly-allocated PCB
 * otherwise. The returned element is extracted from the free PCB list.
 */
pcb_t* allocPcb();


/* PCB queue functions. */
/**
 * Initialize the PCB list pointed to by @c head.
 * @param head Sentinel element of the list
 */
void mkEmptyProcQ(struct list_head *head);
/**
 * @return @c true if @c head refers to an empty list, @c false otherwise.
 */
int emptyProcQ(struct list_head *head);
/**
 * Inserts @c p into the PCB queue pointed to by @c head, such that the
 * process priority is preserved, i.e. the list is ordered in a descending
 * manner with respect to the processes' priority.
 * @param head Pointer to the sentinel of the list to insert to.
 * @param p PCB to be inserted.
 */
void insertProcQ(struct list_head *head, pcb_t *p);
/**
 * @return The head element of @c list, or @c NULL if that list is empty. No
 * removal is performed.
 */
pcb_t* headProcQ(struct list_head *head);
/**
 * Removes the first element from the process queue pointed to by @c head.
 * @param head Sentinel element of the list to remove from.
 * @return The first element of the process queue, or @c NULL if the list
 * contains no element.
 */
pcb_t* removeProcQ(struct list_head *head);
/**
 * Removes the PCB pointed to by @c p from the list @c head.
 * @param head List to remove from
 * @param p PCB pointer to remove from @head. @c p might be found at any
 * arbitrary position within @c head.
 * @return @c p if it was successfully removed from @c head, @c NULL
 * otherwise, i.e. @c p was not within @c head.
 */
pcb_t* outProcQ(struct list_head *head, pcb_t *p);


/* Tree view functions */
int emptyChild(pcb_t *this);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t* removeChild(pcb_t *p);
pcb_t* outChild(pcb_t *p);


#endif
