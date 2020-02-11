#ifndef BKA_ASL_H
#define BKA_ASL_H

#include "types.h"


/* ASL handling functions */
void initASL();
/**
 * @return A pointer to the semaphore whose key equals @c key, or @c NULL if
 * no such semaphore exists.
 */
semd_t* getSemd(int *key);

int insertBlocked(int *key, pcb_t* p);
pcb_t* removeBlocked(int *key);
pcb_t* outBlocked(pcb_t *p);
pcb_t* headBlocked(int *key);
void outChildBlocked(pcb_t *p);


#endif
