#include "exc.h"
#include "io.h"
#include "pcb.h"
#include "string.h"
#include "sched.h"
#include "arch.h"
#include "syscall.h"
#include "test2_aux.h"
#include "utils.h"


/**
 * Initializes the system-wide ready process queue to contain three simple test
 * processes.
 * @see pcb_test_factory
 */
static void sched_queue_init();
/**
 * Returns a PCB set to execute one of the three functions among @c test1(),
 * @c test2() and @c test3(), with an appropriately initialized priority
 * value(s).
 * @param test_no Test type to pick
 * @return A properly initialized PCB whose priority equals @c test_no.
 * @see bka_pcb_init() on how to initialize the PCB.
 */
static pcb_t* pcb_test_factory(unsigned test_no);
/**
 * Initializes the New Areas (fixed memory areas containing callback code to
 * be executed upon system exceptions) to execute specific callback functions.
 * @see sec_int, sec_tlb, sec_trap, sec_sysbk
 */
static void new_areas_init();

/**
 * System Exception Callback (SEC) to be executed during external
 * device and software interrupts.
 */
static void sec_int ();
/**
 * System Exception Callback (SEC) to be executed during TLB exceptions.
 */
static void sec_tlb ();
/**
 * System Exception Callback (SEC) to be executed during program trap
 * exceptions.
 */
static void sec_trap ();
/**
 * System Exception Callback (SEC) to be executed during syscall and
 * breakpoints exceptions.
 */
static void sec_sysbk ();


/**
 * Test program designed to ensure the correctness of scheduling, ROM and
 * exception-handling primitives, whose expected output is a recognizable
 * penguin ASCII-art image.
 */
int main () {
	bka_sched_init();
	bka_pcbs_init();
	sched_queue_init();
	new_areas_init();

	bka_sched_switch_top_hard();

	return 0;
}


void sched_queue_init() {
	int test_no;

	for (test_no = 0; test_no < 3; test_no++)
		bka_sched_ready_enqueue(pcb_test_factory(test_no));
}

pcb_t* pcb_test_factory(unsigned test_no) {
	pcb_t *p = bka_pcb_alloc();
	pfun_t tests[] = {test1, test2, test3};

	if (p) {
		bka_pcb_init(p, tests[test_no], test_no);

		return p;
	}

	return NULL;
}

void new_areas_init() {
	unsigned new_areas[] = {
		INT_NEWAREA, TLB_NEWAREA, PGMTRAP_NEWAREA, SYSBK_NEWAREA
	};
	sec_t callbacks[] = {sec_int, sec_tlb, sec_trap, sec_sysbk};
	unsigned i;
	unsigned const n = BKA_LENGTH(new_areas, unsigned);

	for (i = 0; i < n; i++)
		bka_na_init((state_t *) new_areas[i], callbacks[i]);
}


void sec_int () {
	bka_na_enter(INT_NEWAREA);

	/* If an interrupt is pending on interrupt line 2, i.e. the interval timer: */
	if (getCAUSE() & CAUSE_IP(2)) {
		bka_sched_switch_top((state_t *) INT_OLDAREA);
	} else {
		bka_na_exit(INT_NEWAREA);
	}
}

void sec_tlb () {
	bka_na_enter(TLB_NEWAREA);
	bka_na_exit(TLB_NEWAREA);
}

void sec_trap () {
	bka_na_enter(PGMTRAP_NEWAREA);
	bka_na_exit(PGMTRAP_NEWAREA);
}

void sec_sysbk () {
	state_t *oa;

	bka_na_enter(SYSBK_NEWAREA);

	/* Check what type of interrupt occurred (syscall, breakpoint or other) */
	switch (CAUSE_GET_EXCCODE(getCAUSE())) {
		case EXC_SYS:
			oa = (state_t *) SYSBK_OLDAREA;

#ifdef BKA_ARCH_UMPS
			bka_sys_call(oa->reg_a0, oa->reg_a1, oa->reg_a2, oa->reg_a3);
#elif defined(BKA_ARCH_UARM)
            bka_sys_call(oa->a1, oa->a2, oa->a3, oa->a4);
#endif
			break;
		case EXC_BP:
		default:
			bka_na_exit(SYSBK_NEWAREA);
	}
}
