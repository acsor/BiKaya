#include "callback.h"
#include "io.h"
#include "pcb.h"
#include "string.h"
#include "sys.h"
#include "test2_aux.h"
#include "types.h"
#include "utils.h"


/**
 * @return The boot-up initial process queue.
 */
static list_t pcb_queue_factory();
/**
 * Returns a PCB set to execute one of the three functions among @c test1(),
 * @c test2() and @c test3().
 * @param test_no Test type to pick
 * @return A properly initialized PCB.
 * @see bka_pcb_init() on how to initialize the PCB.
 */
static pcb_t* pcb_test_factory(unsigned test_no);
static void new_areas_init();

static void nac_int ();
static void nac_tlb ();
static void nac_trap ();
static void nac_sysbk ();


int main () {
	list_t proc_queue = pcb_queue_factory();

	new_areas_init();

	return 0;
}


list_t pcb_queue_factory() {
	list_t head;

	initPcbs();
	emptyProcQ(&head);

	insertProcQ(&head, pcb_test_factory(0));
	insertProcQ(&head, pcb_test_factory(1));
	insertProcQ(&head, pcb_test_factory(2));

	return head;
}

pcb_t* pcb_test_factory(unsigned test_no) {
	/* TOOD Implement. */
	return NULL;
}

void new_areas_init() {
	unsigned new_areas[] = {
		INT_NEWAREA, TLB_NEWAREA, PGMTRAP_NEWAREA, SYSBK_NEWAREA
	};
	nac_t callbacks[] = {nac_int, nac_tlb, nac_trap, nac_sysbk};
	unsigned i;
	unsigned const n = BKA_LENGTH(new_areas, unsigned);

	for (i = 0; i < n; i++) {
		bka_na_init((state_t *) new_areas[i], callbacks[i]);
	}
}

void nac_int () {
	/* TODO Implement. */
}

void nac_tlb () {
	/* TODO Implement. */
}

void nac_trap () {
	/* TODO Implement. */
}

void nac_sysbk () {
	/* TODO Implement. */
}
