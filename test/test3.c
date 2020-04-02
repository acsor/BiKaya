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

static void nac_int ();
static void nac_tlb ();
static void nac_trap ();
static void nac_sysbk ();

static int can_print = 1;


int main () {
	termreg_t *const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

#ifdef BKA_ARCH_UMPS
	setSTATUS(getSTATUS() | STATUS_IEc);
	setSTATUS(getSTATUS() & ~STATUS_KUc);
	setSTATUS(getSTATUS() & ~STATUS_VMc);
	setSTATUS(getSTATUS() | 1 << 10);
#elif defined(BKA_ARCH_UARM)
	setSTATUS(STATUS_SYS_MODE);
	setSTATUS(STATUS_ENABLE_INT(getSTATUS()));
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
