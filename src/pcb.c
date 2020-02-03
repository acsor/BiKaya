#ifndef BKA_PCB_H
#define BKA_PCB_H

#include "sys.h"
#include "pcb.h"


/**
 * The PCB table, allocating memory for all the available PCBs.
 */
static pcb_t pcb_table[BKA_MAX_PROC];
/**
 * The free PCB table such that
 * 		* free_pcb_table[i] = 0 if process i is NOT free
 * 		* free_pcb_table[i] = 1 otherwise, i.e. process i IS free
 *
 * 	TODO Couldn't we just leverage on a pcb_t inner field for marking free PCBs?
 */
static unsigned free_pcb_table[BKA_MAX_PROC];


/* TODO Implement missing functions. */
void initPcbs(void) {
	unsigned i;

	for (i = 0; i < BKA_MAX_PROC; i++)
		free_pcb_table[i] = 1;
}

void freePcb(pcb_t *p) {
	// TODO Is index computed correctly?
	unsigned index = (pcb_table - p) / sizeof(pcb_t *);

	free_pcb_table[index] = 1;
}

pcb_t* allocPcb(void) {
	unsigned i;
	pcb_t *out = NULL;

	// Cycle either until i is in range or a free PCB is not found
	for (i = 0; i < BKA_MAX_PROC && free_pcb_table[i] == 0; i++);

	// If we found an eligible free PCB, have out point to it and initialize it
	if (i < BKA_MAX_PROC) {
		out = pcb_table + i;

		// TODO Do we need to reset the next, child and siblings fields? Are
		//  the others ok?
		out->parent = NULL;
		// TODO Urgent! Properly set out->state
		out->priority = 0;
		out->semkey = NULL;

		free_pcb_table[i] = 0;
	}

	return out;
}


#endif
