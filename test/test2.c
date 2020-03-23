#include "io.h"
#include "string.h"
#include "sys.h"
#include "test2_aux.h"
#include "types.h"

#define TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
#define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAMTOP     (RAMBASE + RAMSIZE)


static char to_print[129] = "?\n";

/**
 * Initializes the ROM Reserved Frame new area processor state before receiving
 * an interrupt.
 */
void init_state(state_t *const s);
void hello();


int main () {
	state_t *s = (state_t *) SYSBK_NEWAREA;
	termreg_t * const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	bka_memset(s, 0, sizeof(state_t));
	init_state(s);

	SYSCALL(0, 0, 0, 0);
	bka_term_puts(term0, "to_print = ", to_print, NULL);

	return 0;
}


#ifdef BKA_ARCH_UMPS
void init_state(state_t *const s) {
	s->pc_epc = (unsigned) hello;
	s->reg_sp = RAMTOP;
	s->status = 0;
}
#elif defined(BKA_ARCH_UARM)
	void init_state(state_t *const s) {
	s->sp = RAMTOP;
	s->pc = (unsigned) hello;
	s->cpsr = STATUS_SYS_MODE;
	s->cpsr = STATUS_DISABLE_INT(s->cpsr);
	s->CP15_Control = CP15_DISABLE_VM(s->CP15_Control);
}
#endif

void hello() {
	bka_strcpy(to_print, "Hello! I'm hello().\n");
}
