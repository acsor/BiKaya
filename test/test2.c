#include "callback.h"
#include "io.h"
#include "pcb.h"
#include "string.h"
#include "scheduler.h"
#include "sys.h"
#include "syscall.h"
#include "test2_aux.h"
#include "utils.h"


/**
 * Initializes the system-wide ready process queue to containg three test
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
 * be executed upon certain events) to execute specified callback functions.
 * @see nac_int, nac_tlb, nac_trap, nac_sysbk
 */
static void new_areas_init();

static void nac_int ();
static void nac_tlb ();
static void nac_trap ();
static void nac_sysbk ();


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
	nac_t callbacks[] = {nac_int, nac_tlb, nac_trap, nac_sysbk};
	unsigned i;
	unsigned const n = BKA_LENGTH(new_areas, unsigned);

	for (i = 0; i < n; i++)
		bka_na_init((state_t *) new_areas[i], callbacks[i]);
}

void nac_int () {
#ifdef BKA_ARCH_UMPS
    /* If an interrupt is pending on interrupt line 2, i.e. the interval timer: */
	if (getCAUSE() & CAUSE_IP(2)) {
		bka_sched_switch_top((state_t *) INT_OLDAREA);
	} else {
		bka_na_exit(INT_NEWAREA);
	}
#elif defined(BKA_ARCH_UARM)
    if (CAUSE_IP_GET(getCAUSE(), 2)) {
        bka_sched_switch_top((state_t *) INT_OLDAREA);
    } else {
        bka_na_exit(INT_NEWAREA);
    }
#endif

}

void nac_tlb () {
	bka_na_exit(TLB_NEWAREA);
}

void nac_trap () {
	bka_na_exit(PGMTRAP_NEWAREA);
}

void nac_sysbk () {
	state_t *oa;

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
