#ifndef PCB_H
#define PCB_H

#include "types.h"


/* Free PCB list functions. */
/**
 * Initializes the free PCB list such that it contains all the available
 * processes.
 */
void initPcbs();
/**
 * Initializes the given @pcb_t process descriptor for running. Specifically,
 * the process status will be set in such a way that
 *
 * <ul>
 * 		<li>Interrupts are enabled</li>
 * 		<li>Virtual memory is off</li>
 * 		<li>The process local timer is enabled</li>
 * 		<li>Kernel mode is enabled</li>
 * 		<li>The stack pointer equals <tt>RAMTOP - FRAMESIZE * pid</tt>, where @c
 * 		pid is the Process ID of the given process</li>
 * 		<li>Priority is set to @c 0</li>
 * 		<li>The process program counter points to @c f address.</li>
 * </ul>
 * @param p @c pcb_t instance to be initialized.
 * @param f Process code @c p will execute.
 */
void bka_pcb_init(pcb_t *p, pfun_t f);
/**
 * @param p Pointer to a PCB to be inserted into the free PCB list.
 */
void freePcb(pcb_t *p);
/**
 * @return @c NULL if the free PCB list is empty, a newly-allocated PCB
 * otherwise. The returned element is extracted from the free PCB list.
 */
pcb_t* allocPcb();
/**
 * @return The PID (Process ID) corresponding to the given process control
 * block, or @c BKA_E_INVARG if @c p is not valid.
 */
int bka_pcb_to_pid(pcb_t const * const p);
/**
 * @return The PCB (Process Control Block) corresponding to the given process
 * ID, or @c NULL if the @c pid argument is invalid.
 */
pcb_t * bka_pid_to_pcb(unsigned pid);


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
 * Inserts @c p into the PCB queue pointed to by @c head, so that the
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
/**
 * @return @c 1 if @c this has no children, @c 0 if it has.
 */
int emptyChild(pcb_t *this);
/**
 * Inserts @c child to the end of the children list of @c parent.
 */
void insertChild(pcb_t *parent, pcb_t *child);
/**
 * @param p PCB node to remove the first child from.
 * @return The first element of the children list of @c p, or @c NULL if @c p
 * has no child nodes in its tree view.
 */
pcb_t* removeChild(pcb_t *p);
/**
 * Removes @c p from the children list of its parent.
 *
 * @return @c p itself if it was successfully removed, @c NULL if something
 * goes wrong, e.g. @c p had no parent.
 */
pcb_t* outChild(pcb_t *p);


#endif
