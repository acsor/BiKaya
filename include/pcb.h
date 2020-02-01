#ifndef PCB_H
#define PCB_H

#include "types.h"


/* PCB free list handling functions */
void initPcbs(void);
void freePcb(pcb_t *p);
pcb_t *allocPcb(void);


/**
 * Initialize the PCB list.
 * @param head
 * @return The head of the newly created PCB queue.
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
 * @param head List to insert to.
 * @param p PCB to be inserted.
 */
void insertProcQ(struct list_head *head, pcb_t *p);
/**
 * @return The head element of @c list, or @c NULL if that list is empty. No
 * removal is performed.
 */
pcb_t *headProcQ(struct list_head *head);

pcb_t *removeProcQ(struct list_head *head);
pcb_t *outProcQ(struct list_head *head, pcb_t *p);


/* Tree view functions */
int emptyChild(pcb_t *this);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t *removeChild(pcb_t *p);
pcb_t *outChild(pcb_t *p);

#endif
