#ifndef BKA_TYPES_H
#define BKA_TYPES_H

#include <listx.h>

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
    struct list_head p_next;

    /* Process tree fields */
    struct pcb_t *p_parent;
    struct list_head p_child, p_sib;

    state_t p_s;
    int priority;
    /* Key of the semaphore which the process is eventually blocked on */
    int *p_semkey;
} pcb_t;


/**
 * Semaphore Descriptor (SEMD) data structure.
 */
typedef struct semd_t {
	struct list_head s_next;
	/* Queue of PCBs blocked on the semaphore */
	struct list_head s_procQ;

	/* Semaphore key */
	int *s_key;
} semd_t;


#endif
