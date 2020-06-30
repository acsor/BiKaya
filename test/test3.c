#include "exc.h"
#include "io.h"
#include "pcb.h"
#include "sched.h"
#include "sem.h"
#include "syscall.h"
#include "utils.h"


void test ();


/**
* Initializes the system-wide ready process queue to contain the test process.
* @see proc_factory
*/
static void sched_queue_init();
/**
* @return A PCB initialized to execute the process function @c f, set with an
* original priority of @c original_priority.
* @see bk_pcb_init() on how to initialize the PCB.
*/
static pcb_t* proc_factory(pfun_t f, int original_priority);
/**
 * Initializes the New Areas (fixed memory areas containing callback code to
 * be executed upon system exceptions) to execute specific callback functions.
 * @see sec_int, sec_tlb, sec_trap, sec_sysbk
 */
static void new_areas_init();
static void idle_proc();


/**
 * System Exception Callback (SEC) to be executed during external
 * device and software interrupts.
 */
static void sec_int ();
static void sec_int_handle(void *dev);
static void sec_int_handle_term(termreg_t *dev);
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
	bk_sched_init();
	bk_pcbs_init();
	bk_sem_init();
	sched_queue_init();
	new_areas_init();

	bk_sched_switch_top();

	return 0;
}


void sched_queue_init() {
	bk_sched_enqueue(proc_factory(idle_proc, -1000000));
	bk_sched_enqueue(proc_factory(test, 1));
}

pcb_t* proc_factory(pfun_t f, int original_priority) {
	pcb_t * const p = bk_pcb_alloc();

	bk_pcb_init(p, f, original_priority);

	return p;
}

void new_areas_init() {
	unsigned new_areas[] = {
		INT_NEWAREA, TLB_NEWAREA, PGMTRAP_NEWAREA, SYSBK_NEWAREA
	};
	sec_t callbacks[] = {sec_int, sec_tlb, sec_trap, sec_sysbk};
	unsigned i;
	unsigned const n = BK_LENGTH(new_areas, unsigned);

	for (i = 0; i < n; i++)
		bk_na_init((state_t *) new_areas[i], callbacks[i]);
}

void idle_proc() {
	while (1);
}


void sec_int () {
	state_t *oa = (state_t *) INT_OLDAREA;
	void *dev;

	bk_na_enter(INT_NEWAREA);

	/* As long as there are pending device interrupts, handle them. */
	while ((dev = bk_dev_next_pending()))
		sec_int_handle(dev);

	/* If an interrupt is pending on interrupt line 2, i.e. the interval timer: */
	if (BK_STATE_CAUSE(oa) & CAUSE_IP(2)) {
		/* Save the current process state and switch to the next one. */
		bk_pcb_state_set(bk_sched_curr, oa);
		bk_sched_switch_top();
	}

	bk_na_exit(INT_NEWAREA);
}

void sec_int_handle(void *dev) {
	unsigned line = bk_dev_line(dev);

	if (line == IL_TERMINAL) {
		/* Since handling terminal interrupts is a bit more complex, we
		 * delegate that duty to another function. */
		sec_int_handle_term((termreg_t *) dev);
	} else {
		int *semkey = bk_dev_sem_get(dev, 0);
		dtpreg_t *dtpdev = (dtpreg_t *) dev;
		pcb_t *p;

		/* Perform a V() operation on the semaphore, set the device status as
		 * the syscall return value and acknowledge the interrupt. */
		p = bk_sem_v(semkey);
		bk_syscall_retval(p, dtpdev->status);
		bk_dev_ack2(dev, 0);
	}
}

void sec_int_handle_term(termreg_t *dev) {
	/* Transmit Status (ts) and Receive Status (rs) fields respectively. */
	unsigned const
			ts = dev->transm_status & TERM_ST_MASK,
			rs = dev->recv_status & TERM_ST_MASK;

	if (ts == TERM_ST_IOCE || ts == TERM_ST_TRANSME || ts == TERM_ST_TRANSMITTED) {
		int *semkey = bk_dev_sem_get(dev, 0);
		pcb_t *p;

		p = bk_sem_v(semkey);
		bk_syscall_retval(p, dev->transm_status);
		/** TODO Is it okay to acknowledge error statuses, or should they be
		 * dealt with the reset command? */
		bk_dev_ack2(dev, 0);
	}

	if (rs == TERM_ST_IOCE || rs == TERM_ST_RECVE || rs == TERM_ST_RECEIVED) {
		int *semkey = bk_dev_sem_get(dev, 1);
		pcb_t *p;

		p = bk_sem_v(semkey);
		bk_syscall_retval(p, dev->recv_status);
		/** TODO Is it okay to acknowledge error statuses, or should they be
		 *   dealt with the reset command? */
		bk_dev_ack2(dev, 1);
	}
}

void sec_tlb () {
	bk_na_enter(TLB_NEWAREA);

	if (bk_sched_curr->sp_areas[BK_SP_TLB][1]) {
		bk_sp_enter(BK_SP_TLB);
		bk_kernel_on_exit();

		LDST(bk_sched_curr->sp_areas[BK_SP_TLB][1]);
	} else {
		bk_sched_kill(bk_sched_curr);
		bk_sched_switch_top();
	}

	bk_na_exit(TLB_NEWAREA);
}

void sec_trap () {
	bk_na_enter(PGMTRAP_NEWAREA);

	if (bk_sched_curr->sp_areas[BK_SP_TRAP][1]) {
		bk_sp_enter(BK_SP_TRAP);
		bk_kernel_on_exit();

		LDST(bk_sched_curr->sp_areas[BK_SP_TRAP][1]);
	} else {
		bk_sched_kill(bk_sched_curr);
		bk_sched_switch_top();
	}

	bk_na_exit(PGMTRAP_NEWAREA);
}

void sec_sysbk () {
	state_t *oa = (state_t *) SYSBK_OLDAREA;
	unsigned cause = CAUSE_GET_EXCCODE(BK_STATE_CAUSE(oa));

	if (cause == EXC_SYS)
		return sec_sys();
	else if (cause == EXC_BP)
		return sec_bk();
	else
		PANIC2("sec_sysbk(): cause is neither syscall nor breakpoint\n");
}

void sec_sys () {
	state_t *oa = (state_t *) SYSBK_OLDAREA;

#ifdef BK_ARCH_UARM
	unsigned syscall = oa->a1;
	unsigned arg[] = {oa->a2, oa->a3, oa->a4};
#elif defined(BK_ARCH_UMPS)
	unsigned syscall = oa->reg_a0;
	unsigned arg[] = {oa->reg_a1, oa->reg_a2, oa->reg_a3};
#endif

	bk_na_enter(SYSBK_NEWAREA);

	if (bk_syscall_avail(syscall)) {
		bk_syscall(syscall, arg[0], arg[1], arg[2]);
	} else if (bk_sched_curr->sp_areas[BK_SP_SYSBK][1]) {
		bk_sp_enter(BK_SP_SYSBK);
		bk_kernel_on_exit();

		LDST(bk_sched_curr->sp_areas[BK_SP_SYSBK][1]);
	} else {
		bk_sched_kill(bk_sched_curr);
		bk_sched_resume();
	}

	bk_na_exit(SYSBK_NEWAREA);
}

void sec_bk () {
	bk_na_enter(SYSBK_NEWAREA);

	if (bk_sched_curr->sp_areas[BK_SP_SYSBK][1]) {
		bk_sp_enter(BK_SP_SYSBK);
		bk_kernel_on_exit();

		LDST(bk_sched_curr->sp_areas[BK_SP_SYSBK][1]);
	} else {
		bk_sched_kill(bk_sched_curr);
		bk_sched_resume();
	}

	bk_na_exit(SYSBK_NEWAREA);
}
