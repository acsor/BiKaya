#include "callback.h"
#include "io.h"
#include "pcb.h"
#include "string.h"
#include "scheduler.h"
#include "sys.h"
#include "test2_aux.h"
#include "types.h"
#include "utils.h"

#ifdef BKA_ARCH_UMPS
#include "umps/cp0.h"
#endif


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

static int can_print = 1;

int main () {
	termreg_t *const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

#ifdef BKA_ARCH_UMPS
	setSTATUS(getSTATUS() | STATUS_IEc);
	setSTATUS(getSTATUS() & !STATUS_KUc);
	setSTATUS(getSTATUS() & !STATUS_VMc);
	setSTATUS(getSTATUS() | 1 << 9);
#elif defined(BKA_ARCH_UARM)
	setSTATUS(STATUS_INT_MODE);
	setSTATUS(STATUS_DISABLE_INT(getSTATUS()));
	setCONTROL(CP15_DISABLE_VM(getCONTROL()));
#endif

	bka_interval_timer_set(500, BKA_STU_MILLI);
	bka_na_init((state_t*) INT_NEWAREA, nac_int);

	while (1) {
		if (can_print) {
			bka_term_puts(term0, "This is the interval timer!\n", NULL);
			can_print = 0;
		}
	}

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
	pcb_t *p = allocPcb();
	pfun_t tests[] = {test1, test2, test3};

	if (p) {
		bka_pcb_init(p, tests[test_no]);

		return p;
	}

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
	bka_interval_timer_set(500, BKA_STU_MILLI);
	can_print = 1;

	bka_na_exit(INT_NEWAREA);
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
