#ifndef BKA_ARCH_H
#define BKA_ARCH_H


#ifdef BKA_ARCH_UMPS
#include "umps/arch.h"
#include "umps/cp0.h"
#include "umps/libumps.h"
#include "umps/types.h"
#elif defined(BKA_ARCH_UARM)
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#include "uarm/uARMtypes.h"
#endif


/* ERROR CONSTANTS. */
/**
 * No error occurred.
 */
#define BKA_E_OK		0
/**
 * A "generic" kind of error, for primitives that do not want to further
 * specify the kind of error occurred.
 */
#define BKA_E_GEN		-1
/**
 * A null-pointer error, relating for example to function arguments.
 */
#define BKA_E_NULLPTR	-2
/**
 * An invalid argument error.
 */
#define BKA_E_INVARG	-3

/**
 * Utility macro to emit an error message and panic the calling code.
 */
#define PANIC2(msg)	{ bka_term_puts2(0, msg, NULL); PANIC(); }

/* OTHER CONSTANTS. */
#ifndef NULL
#define NULL	((void*)0)
#endif


/* CODE INHERITED FROM p1.5test_bikaya_v0.c. */
#define BKA_TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define BKA_TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define BKA_RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define BKA_RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define BKA_RAMTOP     (BKA_RAMBASE + BKA_RAMSIZE)


/* CODE INHERITED FROM const.h, PROVIDED BY UNIVERSITY TUTOR AND PROFESSOR. */
/* Maximum number of processes concurrently active in the system. */
#define BKA_MAX_PROC 20
#define MAXPROC BKA_MAX_PROC

/* Num. of usermode processes (not including master proc) and system daemons */
#define UPROCMAX 3

#define	HIDDEN  static
#define	TRUE 	1
#define	FALSE	0
#define ON 	    1
#define OFF 	0
#define EOS     '\0'

/* Maximum number of devices per interrupt line */
#ifndef DEV_PER_INT
#define DEV_PER_INT 8
#endif
#define BKA_DEV_PER_INT DEV_PER_INT


#endif
