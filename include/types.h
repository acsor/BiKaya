#ifndef BKA_TYPES_H
#define BKA_TYPES_H

#include <list.h>

#ifdef BKA_ARCH_UMPS
#include "umps/types.h"
#endif

#ifdef BKA_ARCH_UARM
#define UARM_MACHINE_COMPILING
#include "uarm/uARMtypes.h"
#endif


typedef unsigned int memaddr_t;

/**
 * Process Control Block (PCB) data structure.
 */
typedef struct pcb_t {
    /* Process queue fields */
    struct list_head next;

    /* Process tree fields */
    struct pcb_t *parent;
    struct list_head child, siblings;

    state_t state;
    int priority;
    /* Key of the semaphore which the process is eventually blocked on */
    int *semkey;
} pcb_t;


/**
 * Semaphore Descriptor (SEMD) data structure.
 */
typedef struct semd_t {
	struct list_head next;
	/* Queue of PCBs blocked on the semaphore */
	struct list_head proc_queue;

	/* Semaphore key */
	int *key;
} semd_t;


#endif
