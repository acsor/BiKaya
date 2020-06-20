#include "exc.h"
#include "pcb.h"
#include "sched.h"
#include "sem.h"
#include "syscall.h"
#include "utils.h"


void test ();


/**
 * Initializes the system-wide ready process queue to contain the test process.
 * @see pcb_test_factory
 */
static void sched_queue_init();
/**
 * @return The only viable PCB whose process will begin after the
 * initialization phase.
 * @see bka_pcb_init() on how to initialize the PCB.
 */
static pcb_t* pcb_test_factory();
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
static void sec_sys ();
static void sec_bk ();


int main () {
	bka_sched_init();
	bka_pcbs_init();
	bka_sem_init();
	sched_queue_init();
	new_areas_init();

	bka_sched_switch_top();

	return 0;
}


void sched_queue_init() {
	bka_sched_enqueue(pcb_test_factory());
}

pcb_t* pcb_test_factory() {
	pcb_t * const p = bka_pcb_alloc();

	bka_pcb_init(p, test, 1);

	return p;
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
	state_t *oa = (state_t *) INT_OLDAREA;

	bka_na_enter(INT_NEWAREA);

	/* If an interrupt is pending on interrupt line 2, i.e. the interval timer: */
	if (BKA_STATE_CAUSE(oa) & CAUSE_IP(2)) {
		/* Save the current process state and switch to the next one. */
		bka_pcb_state_set(bka_sched_curr, oa);
		bka_sched_switch_top();
	} else {
		bka_na_exit(INT_NEWAREA);
	}
}

void sec_tlb () {
	bka_na_enter(TLB_NEWAREA);

	/* TODO How do we know when we are exiting out of a spec passup new area?
	 *  Until this is not sorted out, we'll consider time spent in the spec
	 *  passup new area as user time. */
	if (bka_sched_curr->sp_areas[BKA_SP_TLB][1]) {
		bka_sp_enter(BKA_SP_TLB);
		bka_kernel_on_exit();

		LDST(bka_sched_curr->sp_areas[BKA_SP_TLB][1]);
	} else {
		bka_sched_kill(bka_sched_curr);
		bka_sched_switch_top();
	}

	bka_na_exit(TLB_NEWAREA);
}

void sec_trap () {
	bka_na_enter(PGMTRAP_NEWAREA);

	if (bka_sched_curr->sp_areas[BKA_SP_TRAP][1]) {
		bka_sp_enter(BKA_SP_TRAP);
		bka_kernel_on_exit();

		LDST(bka_sched_curr->sp_areas[BKA_SP_TRAP][1]);
	} else {
		bka_sched_kill(bka_sched_curr);
		bka_sched_switch_top();
	}

	bka_na_exit(PGMTRAP_NEWAREA);
}

void sec_sysbk () {
	state_t *oa = (state_t *) SYSBK_OLDAREA;
	unsigned cause = CAUSE_GET_EXCCODE(BKA_STATE_CAUSE(oa));

	if (cause == EXC_SYS) {
		return sec_sys();
	} else if (cause == EXC_BP) {
		return sec_bk();
	} else {
		/* TODO Print out error message. */
		PANIC();
	}
}

void sec_sys () {
	state_t *oa = (state_t *) SYSBK_OLDAREA;

#ifdef BKA_ARCH_UARM
	unsigned syscall = oa->a1;
	unsigned arg[] = {oa->a2, oa->a3, oa->a4};
#elif defined(BKA_ARCH_UMPS)
	unsigned syscall = oa->reg_a0;
	unsigned arg[] = {oa->reg_a1, oa->reg_a2, oa->reg_a3};
#endif

	bka_na_enter(SYSBK_NEWAREA);

	if (bka_syscall_avail(syscall)) {
		bka_syscall(syscall, arg[0], arg[1], arg[2]);
	} else if (bka_sched_curr->sp_areas[BKA_SP_SYSBK][1]) {
		bka_sp_enter(BKA_SP_SYSBK);
		bka_kernel_on_exit();

		LDST(bka_sched_curr->sp_areas[BKA_SP_SYSBK][1]);
	} else {
		bka_sched_kill(bka_sched_curr);
		bka_sched_resume();
	}

	bka_na_exit(SYSBK_NEWAREA);
}

void sec_bk () {
	bka_na_enter(SYSBK_NEWAREA);

	if (bka_sched_curr->sp_areas[BKA_SP_SYSBK][1]) {
		bka_sp_enter(BKA_SP_SYSBK);
		bka_kernel_on_exit();

		LDST(bka_sched_curr->sp_areas[BKA_SP_SYSBK][1]);
	} else {
		bka_sched_kill(bka_sched_curr);
		bka_sched_resume();
	}

	bka_na_exit(SYSBK_NEWAREA);
}
