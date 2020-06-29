#ifndef BK_ARCH_H
#define BK_ARCH_H


#ifdef BK_ARCH_UMPS
#include "umps/arch.h"
#include "umps/cp0.h"
#include "umps/libumps.h"
#include "umps/types.h"
#elif defined(BK_ARCH_UARM)
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#include "uarm/uARMtypes.h"
#endif


/* ERROR CONSTANTS. */
/**
 * No error occurred.
 */
#define BK_E_OK		0
/**
 * A "generic" kind of error, for primitives that do not want to further
 * specify the kind of error occurred.
 */
#define BK_E_GEN		-1
/**
 * A null-pointer error, relating for example to function arguments.
 */
#define BK_E_NULLPTR	-2
/**
 * An invalid argument error.
 */
#define BK_E_INVARG	-3

/**
 * Utility macro to emit an error message and panic the calling code.
 */
#define PANIC2(msg)	{ bk_term_puts2(0, msg, NULL); PANIC(); }

/* OTHER CONSTANTS. */
#ifndef NULL
#define NULL	((void*)0)
#endif


/* CODE INHERITED FROM p1.5test_bikaya_v0.c. */
#define BK_TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define BK_TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define BK_RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define BK_RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define BK_RAMTOP     (BK_RAMBASE + BK_RAMSIZE)


/* CODE INHERITED FROM const.h, PROVIDED BY UNIVERSITY TUTOR AND PROFESSOR. */
/* Maximum number of processes concurrently active in the system. */
#define BK_MAX_PROC 20
#define MAXPROC BK_MAX_PROC

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
#define BK_DEV_PER_INT DEV_PER_INT


#endif
