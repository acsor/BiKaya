#include "exc.h"
#include "io.h"
#include "pcb.h"
#include "string.h"


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


int main () {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	bka_term_puts(term0, "I print, therefore I work.\n", NULL);

	return 0;
}


void sec_int () {
	/* TODO Implement. */
	bka_na_enter(INT_NEWAREA);
	bka_na_exit(INT_NEWAREA);
}

void sec_tlb () {
	bka_na_enter(TLB_NEWAREA);
	bka_na_exit(TLB_NEWAREA);
}

void sec_trap () {
	/* TODO Implement. */
	bka_na_enter(PGMTRAP_NEWAREA);
	bka_na_exit(PGMTRAP_NEWAREA);
}

void sec_sysbk () {
	/* TODO Implement. */
	bka_na_enter(SYSBK_NEWAREA);
	bka_na_exit(SYSBK_NEWAREA);
}
