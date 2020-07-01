/*****************************************************************************
 * Copyright 2004, 2005 Michael Goldweber, Davide Brini.                     *
 * Modified 2012 by Marco Di Felice                                          *
 * Modified 2018 by Renzo Davoli
 *                                                                           *
 * This file is part of BIKAYA.                                              *
 *                                                                           *
 * bikaya is free software;you can redistribute it and/or modify it under the*
 * terms of the GNU General Public License as published by the Free Software *
 * Foundation; either version 2 of the License, or (at your option) any      *
 * later version.                                                            *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.                  *
 *****************************************************************************/

/**
 *	Test program for the sem and PCB modules (phase 1).
 *
 *	Produces progress messages on terminal 0 in addition to the array
 *	``okbuf[]''. Error messages will also appear on terminal 0 in addition to
 *	the array ``errbuf[]''. Aborts as soon as an error is detected.
 */
#include "list.h"

#ifdef BK_ARCH_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#endif
#ifdef BK_ARCH_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#endif

#include "arch.h"
#include "pcb.h"
#include "sem.h"

#define MAXSEM MAXPROC

#define MAX_PCB_PRIORITY     10
#define MIN_PCB_PRIORITY     0
#define DEFAULT_PCB_PRIORITY 5

char okbuf[2048]; /* sequence of progress messages */
char errbuf[128]; /* contains reason for failing */
char msgbuf[128]; /* nonrecoverable error message before shut down */


int     onesem;
pcb_t * procp[MAXPROC], *p, *q, *maxproc, *minproc, *proc;
semd_t *semd[MAXSEM];
int     sem[MAXSEM + 1];

struct list_head qa;
char *           mp = okbuf;

#define ST_READY       1
#define ST_BUSY        3
#define ST_TRANSMITTED 5

#define CMD_ACK      1
#define CMD_TRANSMIT 2

#define CHAR_OFFSET      8
#define TERM_STATUS_MASK 0xFF


/******************************************************************************
 * I/O Routines to write on a terminal
 ******************************************************************************/

/* This function returns the terminal transmitter status value given its address */
static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

/* This function prints a string on specified terminal and returns TRUE if
 * print was successful, FALSE if not   */
unsigned int termprint(char *str, unsigned int term) {
    termreg_t *term_reg;

    unsigned int stat;
    unsigned int cmd;

    unsigned int error = FALSE;

    if (term < DEV_PER_INT) {
        term_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, term);

        /* test device status */
        stat = tx_status(term_reg);
        if ((stat == ST_READY) || (stat == ST_TRANSMITTED)) {
            /* device is available */

            /* print cycle */
            while ((*str != '\0') && (!error)) {
                cmd                      = (*str << CHAR_OFFSET) | CMD_TRANSMIT;
                term_reg->transm_command = cmd;

                /* busy waiting */
                while ((stat = tx_status(term_reg)) == ST_BUSY)
                    ;

                /* end of wait */
                if (stat != ST_TRANSMITTED) {
                    error = TRUE;
                } else {
                    /* move to next char */
                    str++;
                }
            }
        } else {
            /* device is not available */
            error = TRUE;
        }
    } else {
        /* wrong terminal device number */
        error = TRUE;
    }

    return (!error);
}


/* This function places the specified character string in okbuf and
 *	causes the string to be written out to terminal0 */
void addokbuf(char *strp) {

    termprint(strp, 0);
}


/* This function places the specified character string in errbuf and
 *	causes the string to be written out to terminal0.  After this is done
 *	the system shuts down with a panic message */
void adderrbuf(char *strp) {

    termprint(strp, 0);

    PANIC();
}


/******************************************************************************
 * Main Test File
 ******************************************************************************/


int main() {
    int i;

	bk_pcbs_init();
    addokbuf("Initialized Process Control Blocks   \n");

    /* Check bk_pcb_alloc */
    for (i = 0; i < MAXPROC; i++) {
        if ((procp[i] = bk_pcb_alloc()) == NULL)
            adderrbuf("bk_pcb_alloc(): unexpected NULL   ");
    }

    if (bk_pcb_alloc() != NULL) {
        adderrbuf(" ERROR: bk_pcb_alloc(): allocated more than MAXPROC entries   ");
    }
    addokbuf(" bk_pcb_alloc test OK   \n");


    /* Return the last 10 entries back to free list */
    for (i = 10; i < MAXPROC; i++)
		bk_pcb_free(procp[i]);

    addokbuf(" Added 10 entries to the free PCB list   \n");

    /* Create a 10-element process queue */
    INIT_LIST_HEAD(&qa);

    if (!bk_pcb_queue_isempty(&qa))
        adderrbuf("ERROR: bk_pcb_queue_isempty(qa): unexpected FALSE   ");

    addokbuf("Testing bk_pcb_queue_ins ...   \n");

    for (i = 0; i < 10; i++) {
        if ((q = bk_pcb_alloc()) == NULL)
            adderrbuf("ERROR: bk_pcb_alloc(): unexpected NULL while insert   ");
        switch (i) {
            case 3:
                q->priority = DEFAULT_PCB_PRIORITY;
                proc        = q;
                break;
            case 4:
                q->priority = MAX_PCB_PRIORITY;
                maxproc     = q;
                break;
            case 5:
                q->priority = MIN_PCB_PRIORITY;
                minproc     = q;
                break;
            default:
                q->priority = DEFAULT_PCB_PRIORITY;
                break;
        }
		bk_pcb_queue_ins(&qa, q);
    }

    addokbuf("Test bk_pcb_queue_ins: OK. Inserted 10 elements \n");

    if (bk_pcb_queue_isempty(&qa))
        adderrbuf("ERROR: bk_pcb_queue_isempty(qa): unexpected TRUE");

    /* Check bk_pcb_queue_rm and bk_pcb_queue_head */
    if (bk_pcb_queue_head(&qa) != maxproc)
        adderrbuf("ERROR: bk_pcb_queue_head(qa) failed   ");

    /* Removing an element from ProcQ */
    q = bk_pcb_queue_rm(&qa, proc);

    if ((q == NULL) || (q != proc)) {
		adderrbuf("ERROR: bk_pcb_queue_rm(&qa, proc) failed to remove the entry   ");
	}

	bk_pcb_free(q);

    /* Removing the first element from ProcQ */
    q = bk_pcb_queue_pop(&qa);
    if (q == NULL || q != maxproc) {
		adderrbuf("ERROR: bk_pcb_queue_pop(&qa, midproc) failed to remove the elements in the right order   ");
	}
	bk_pcb_free(q);

    /* Removing other 7 elements  */
    addokbuf(" Testing bk_pcb_queue_pop ...   \n");
    for (i = 0; i < 7; i++) {
        if ((q = bk_pcb_queue_pop(&qa)) == NULL)
            adderrbuf("bk_pcb_queue_pop(&qa): unexpected NULL   ");
		bk_pcb_free(q);
    }

    /* Removing the last element */
    q = bk_pcb_queue_pop(&qa);
    if (q != minproc) {
		adderrbuf("ERROR: bk_pcb_queue_pop(): failed on last entry   ");
	}
	bk_pcb_free(q);

    if (bk_pcb_queue_pop(&qa) != NULL)
        adderrbuf("ERROR: bk_pcb_queue_pop(&qa): removes too many entries   ");

    if (!bk_pcb_queue_isempty(&qa))
        adderrbuf("ERROR: bk_pcb_queue_isempty(qa): unexpected FALSE   ");

    addokbuf(" Test bk_pcb_queue_ins(), bk_pcb_queue_pop() and bk_pcb_queue_isempty(): OK   \n");
    addokbuf(" Test process queues module: OK      \n");

    addokbuf(" Testing process trees...\n");

    if (!bk_pcb_tree_isempty(procp[2]))
        adderrbuf("ERROR: bk_pcb_tree_isempty: unexpected FALSE   ");

    /* make procp[1],procp[2],procp[3], procp[7] children of procp[0] */
    addokbuf("Inserting...   \n");
    bk_pcb_tree_push(procp[0], procp[1]);
    bk_pcb_tree_push(procp[0], procp[2]);
    bk_pcb_tree_push(procp[0], procp[3]);
    bk_pcb_tree_push(procp[0], procp[7]);
    addokbuf("Inserted 2 children of pcb0  \n");

    /* make procp[8],procp[9] children of procp[7] */
    bk_pcb_tree_push(procp[7], procp[8]);
    bk_pcb_tree_push(procp[7], procp[9]);
    addokbuf("Inserted 2 children of pcb7  \n");

    if (bk_pcb_tree_isempty(procp[0]))
        adderrbuf("ERROR: bk_pcb_tree_isempty(procp[0]): unexpected TRUE   ");

    if (bk_pcb_tree_isempty(procp[7]))
        adderrbuf("ERROR: bk_pcb_tree_isempty(procp[0]): unexpected TRUE   ");

    /* Check bk_pcb_tree_parentrm */
    q = bk_pcb_tree_parentrm(procp[1]);
    if (q == NULL || q != procp[1])
        adderrbuf("ERROR: bk_pcb_tree_parentrm(procp[1]) failed ");

    q = bk_pcb_tree_parentrm(procp[8]);
    if (q == NULL || q != procp[8])
        adderrbuf("ERROR: bk_pcb_tree_parentrm(procp[8]) failed ");

    /* Check bk_pcb_tree_pop */
    q = bk_pcb_tree_pop(procp[0]);
    if (q == NULL || q != procp[2])
        adderrbuf("ERROR: bk_pcb_tree_pop(procp[0]) failed ");

    q = bk_pcb_tree_pop(procp[7]);
    if (q == NULL || q != procp[9])
        adderrbuf("ERROR: bk_pcb_tree_pop(procp[7]) failed ");

    q = bk_pcb_tree_pop(procp[0]);
    if (q == NULL || q != procp[3])
        adderrbuf("ERROR: bk_pcb_tree_pop(procp[0]) failed ");

    q = bk_pcb_tree_pop(procp[0]);
    if (q == NULL || q != procp[7])
        adderrbuf("ERROR: bk_pcb_tree_pop(procp[0]) failed ");


    if (bk_pcb_tree_pop(procp[0]) != NULL)
        adderrbuf("ERROR: bk_pcb_tree_pop(): removes too many children   ");

    if (!bk_pcb_tree_isempty(procp[0]))
        adderrbuf("ERROR: bk_pcb_tree_isempty(procp[0]): unexpected FALSE   ");

    addokbuf("Test: bk_pcb_tree_push(), bk_pcb_tree_pop() and bk_pcb_tree_isempty() OK   \n");
    addokbuf("Testing process tree module OK      \n");


	bk_pcb_free(procp[0]);
	bk_pcb_free(procp[1]);
	bk_pcb_free(procp[2]);
	bk_pcb_free(procp[3]);
	bk_pcb_free(procp[4]);
	bk_pcb_free(procp[5]);
	bk_pcb_free(procp[6]);
	bk_pcb_free(procp[7]);
	bk_pcb_free(procp[8]);
	bk_pcb_free(procp[9]);


    /* check sem */
	bk_sem_init();
    addokbuf("Initializing active semaphore list   \n");

    /* check bk_sem_dequeue and bk_sem_enqueue */
    addokbuf(" Test bk_sem_enqueue(): test #1 started  \n");
    for (i = 10; i < MAXPROC; i++) {
        procp[i] = bk_pcb_alloc();
        if (bk_sem_enqueue(&sem[i], procp[i]))
            adderrbuf("ERROR: bk_sem_enqueue() test#1: unexpected TRUE   ");
    }

    addokbuf("Test bk_sem_enqueue(): test #2 started  \n");
    for (i = 0; i < 10; i++) {
        procp[i] = bk_pcb_alloc();
        if (bk_sem_enqueue(&sem[i], procp[i]))
            adderrbuf("ERROR:bk_sem_enqueue() test #2: unexpected TRUE   ");
    }

    /* check if semaphore descriptors are returned to the free list */
    p = bk_sem_dequeue(&sem[11]);
    if (bk_sem_enqueue(&sem[11], p))
        adderrbuf("ERROR: bk_sem_dequeue(): fails to return to free list   ");

    if (bk_sem_enqueue(&sem[MAXSEM], procp[9]) == FALSE)
        adderrbuf("ERROR: bk_sem_enqueue(): inserted more than MAXPROC   ");

    addokbuf("Test bk_sem_dequeue(): test started   \n");
    for (i = 10; i < MAXPROC; i++) {
        q = bk_sem_dequeue(&sem[i]);
        if (q == NULL)
            adderrbuf("ERROR: bk_sem_dequeue(): wouldn't remove   ");
        if (q != procp[i])
            adderrbuf("ERROR: bk_sem_dequeue(): removed wrong element   ");
    }

    if (bk_sem_dequeue(&sem[11]) != NULL)
        adderrbuf("ERROR: bk_sem_dequeue(): removed nonexistent blocked proc   ");

    addokbuf("Test bk_sem_enqueue() and bk_sem_dequeue() ok   \n");

    if (bk_sem_head(&sem[11]) != NULL)
        adderrbuf("ERROR: bk_sem_head(): nonNULL for a nonexistent queue   ");

    if ((q = bk_sem_head(&sem[9])) == NULL)
        adderrbuf("ERROR: bk_sem_head(1): NULL for an existent queue   ");
    if (q != procp[9])
        adderrbuf("ERROR: bk_sem_head(1): wrong process returned   ");
    p = bk_sem_pcb_rm(q);
    if (p != q)
        adderrbuf("ERROR: bk_sem_pcb_rm(1): couldn't remove from valid queue   ");

    /* Creating a 2-layer tree */
    bk_pcb_tree_push(procp[0], procp[1]);
    bk_pcb_tree_push(procp[0], procp[2]);
    bk_pcb_tree_push(procp[0], procp[3]);
    bk_pcb_tree_push(procp[3], procp[4]);

    /* Testing bk_sem_pcb_rm_desc */
	bk_sem_pcb_rm_desc(procp[0]);

    if (bk_sem_head(&sem[0]) != NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): nonNULL for a nonexistent queue (0)  ");
    if (bk_sem_head(&sem[1]) != NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): nonNULL for a nonexistent queue (1)  ");
    if (bk_sem_head(&sem[2]) != NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): nonNULL for a nonexistent queue  (2) ");
    if (bk_sem_head(&sem[3]) != NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): nonNULL for a nonexistent queue (3)  ");
    if (bk_sem_head(&sem[4]) != NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): nonNULL for a nonexistent queue (4)  ");
    if (bk_sem_head(&sem[5]) == NULL)
        adderrbuf("ERROR: bk_sem_pcb_rm_desc(): NULL for an existent queue  (5) ");

    addokbuf("Test bk_sem_head() and bk_sem_pcb_rm(): OK   \n");

    addokbuf("sem module OK   \n");
    addokbuf("So Long and Thanks for All the Fish\n");

    return 0;
}
