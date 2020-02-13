#include "asl.h"
#include "sys.h"


/**
 * Array di SEMD con dimensione massima BKA_MAX_PROC?
 * TODO O una dimensione diversa?.
 */
static semd_t semd_table[BKA_MAX_PROC];

/**
 * Array di SEMD liberi tale che 
 * 		* free_semd_table[i] = 0 se processo non libero
 * 		* free_semd_table[i] = 1 se libero
 */
static unsigned free_semd_table[BKA_MAX_PROC];

/**
 * TODO bisogna inizializzare questi campi?
 * struct list_head s_next;
 * int *s_key;
 * struct list_head s_procQ;
*/
void initASL() {
	int i;

	for (i = 0; i < BKA_MAX_PROC; i++)
		free_semd_table[i] = 1;
}

/*TODO verificare il funzionamento*/

semd_t* getSemd(int *key) {
	int i;
	semd_t *result;

	for (i = 0; i < BKA_MAX_PROC && (*(semd_table[i].key) != *key); i++);

	if (i < BKA_MAX_PROC) {
		result = &semd_table[i]; //passa l'indirizzo del semd cercato
		return result;
	}
	return NULL;
}

/* TODO Implement */
int insertBlocked (int *key,pcb_t* p) {
	return 0;
}

/* TODO Verificare funzionamento */
pcb_t* removeBlocked(int *key) {
	int i;
	pcb_t *out;
	
	for (i = 0; i < BKA_MAX_PROC && (*(semd_table[i].key) != *key); i++);
	if (i < BKA_MAX_PROC) {
		out = container_of(semd_table[i],semd_t,proc_queue);
		list_del(out);	
		if(list_empty(semd_table[i].proc_queue)){	//se coda dei processi bloccati è diventata vuota
			free_semd_table[i] = 1;
		}
		return out;
	}
	return NULL;
}

/* TODO Implement */
pcb_t* outBlocked(pcb_t *p) {
	return NULL;
}

/* TODO Is this semantically correct? */
pcb_t* headBlocked(int *key) {
	int i;
	pcb_t *result;

	for (i = 0; i < BKA_MAX_PROC && (*(semd_table[i].key) != *key); i++);

	if (i < BKA_MAX_PROC) {
		result = container_of(semd_table[i],semd_t,proc_queue);

		return list_empty(result) ? NULL: result;
	}

	return NULL;
}

void outChildBlocked(pcb_t *p) {

}
