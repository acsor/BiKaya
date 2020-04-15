#include "arch.h"
#include "exc.h"
#include "io.h"
#include "pcb.h"
#include "sched.h"
#include "syscall.h"
#include "utils.h"

#define TEST_STEPS	6


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

static unsigned get_microseconds();
static void delay_ms(unsigned ms);

static void proc_test1();
static void proc_test2();
static void proc_test3();


volatile int test1_baton[TEST_STEPS + 1] = {0};
volatile int test2_baton[TEST_STEPS + 1] = {0};
volatile int test3_baton[TEST_STEPS + 1] = {0};

static char *to_print[] = {
		"1                        \n","2          _nnnn_        \n",
		"3         dGGGGMMb       \n", "4        @p~qp~~qMb      \n",
		"5        M|@||@) M|      \n",  "6        @,----.JM|      \n",
		"7       JS^\\__/  qKL     \n", "8      dZP        qKRb   \n",
		"9     dZP          qKKb  \n", "10   fZP            SMMb \n",
		"11   HZM            MMMM \n",  "12   FqM            MMMM \n",
		"13 __| '.        |\\dS'qML\n", "14 |    `.       | `' \\Zq\n",
		"15_)      \\.___.,|     .'\n", "16\\____   )MMMMMP|   .'  \n",
		"17     `-'       `--'    \n",  "18                       \n",
};


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
	pfun_t tests[] = {proc_test1, proc_test2, proc_test3};

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
	state_t *oa = (state_t *) INT_OLDAREA;

	bka_na_enter(INT_NEWAREA);

	/* If an interrupt is pending on interrupt line 2, i.e. the interval timer: */
	if (BKA_STATE_CAUSE(oa) & CAUSE_IP(2)) {
		bka_sched_switch_top(oa);
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
	state_t *oa = (state_t *) SYSBK_OLDAREA;

	/* Check what type of interrupt occurred (syscall, breakpoint or other) */
	switch(CAUSE_GET_EXCCODE(BKA_STATE_CAUSE(oa))) {
		case EXC_SYS:

#ifdef BKA_ARCH_UARM
			bka_sys_call(oa->a1, oa->a2, oa->a3, oa->a4);
#elif defined(BKA_ARCH_UMPS)
			bka_sys_call(oa->reg_a0, oa->reg_a1, oa->reg_a2, oa->reg_a3);
#endif

			break;
		case EXC_BP:
		default:
			bka_na_exit(SYSBK_NEWAREA);
	}
}


static unsigned get_microseconds() {
	return BKA_TOD_LO / BKA_TIME_SCALE;
}

static void delay_ms(unsigned ms) {
	unsigned start = get_microseconds();

	while (get_microseconds() - start <= ms * 1000);
}

void proc_test1() {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);
	int i = 0;

	bka_term_puts(term0, "Entering test1!\n", NULL);

	for (i = 0; i < TEST_STEPS; i++) {
		while (test3_baton[i] == 0)
			;

		bka_term_puts(term0, to_print[i * 3], NULL);
		delay_ms(100);
		test1_baton[i] = 1;
	}

	while (test3_baton[TEST_STEPS] == 0)
		;

	bka_term_puts(term0, "Good job from test1\n", NULL);
	test1_baton[TEST_STEPS] = 1;
	/* TODO Once phase 1.5 is over, implement BKA_SYS_KILL with bka_sys_kill
	 * instead, feeding the pid of the process to kill here. */
	SYSCALL(BKA_SYS_KILL, 0, 0, 0);
}

void proc_test2() {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);
	int i = 0;

	bka_term_puts(term0, "Entering test2!\n", NULL);

	for (i = 0; i < TEST_STEPS; i++) {
		while (test1_baton[i] == 0)
			;

		bka_term_puts(term0, to_print[i * 3 + 1], NULL);
		delay_ms(100);
		test2_baton[i] = 1;
	}

	while (test1_baton[TEST_STEPS] == 0)
		;

	bka_term_puts(term0, "Good job from test2\n", NULL);
	test2_baton[TEST_STEPS] = 1;
	SYSCALL(BKA_SYS_KILL, 0, 0, 0);
}

void proc_test3() {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);
	int i = 0;

	bka_term_puts(term0, "Entering test3!\n", NULL);
	test3_baton[0] = 1;

	for (i = 0; i < TEST_STEPS; i++) {
		while (test2_baton[i] == 0)
			;

		bka_term_puts(term0, to_print[i * 3 + 2], NULL);
		delay_ms(100);
		test3_baton[i + 1] = 1;
	}

	while (test2_baton[TEST_STEPS] == 0)
		;

	bka_term_puts(term0, "Good job from test3\n", NULL);
	SYSCALL(BKA_SYS_KILL, 0, 0, 0);
}
