#ifndef BKA_ASL_H
#define BKA_ASL_H
#include <types_bikaya.h>


/**
 * Array di SEMD con dimensione massima BKA_MAX_PROC? TODO O una dimensione diversa?.
 */
static semd_t semd_table[BKA_MAX_PROC];


/**
 * Array di SEMD liberi tale che 
 * 		* free_semd_table[i] = 0 se processo non libero
 * 		* free_semd_table[i] = 1 se libero
 */
static unsigned free_semd_table[BKA_MAX_PROC];



/**TODO bisogna inizializzare questi campi?
	struct list_head s_next;
    int *s_key;
    struct list_head s_procQ;
*/
void initASL() {
	int i;
	
	for(i = 0; i < BKA_MAX_PROC; i++)
		free_semd_table[i] = 1;
		
}

semd_t* getSemd(int *key);
int insertBlocked(int *key,pcb_t* p);
pcb_t* removeBlocked(int *key);
pcb_t* outBlocked(pcb_t *p);

/**
pcb_t* headBlocked(int *key) {
	int i;
	pcb_t *result;
	
	for(i=0;i < BKA_MAX_PROC && semd_table[i]->s_key != *key; i++);
	
	if(i < BKA_MAX_PROC){
		result = container_of(semd_table[i]-,semd_t,s_procQ)
		if(list_empty(result))
			return NULL;
		else
			return result;
	}
	else
		return NULL;

}

**/







void outChildBlocked(pcb_t *p);









#endif
