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


static char message[129] = "?\n";
static termreg_t * const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

/**
 * Initializes the ROM Reserved Frame new area processor state before receiving
 * an interrupt.
 */
void init_state(state_t *const s);
static void sys_callback();


int main () {
	state_t *s = (state_t *) SYSBK_NEWAREA;

	init_state(s);

	bka_term_puts(term0, "Invoking SYSCALL()...\n", NULL);
	SYSCALL(0, 0, 0, 0);
	bka_term_puts(term0, "message = ", message, NULL);

	return 0;
}


#ifdef BKA_ARCH_UMPS
void init_state(state_t *const s) {
	bka_memset(s, 0, sizeof(state_t));
	s->pc_epc = (unsigned) sys_callback;
	s->reg_sp = RAMTOP;
}
#elif defined(BKA_ARCH_UARM)
void init_state(state_t *const s) {
	bka_memset(s, 0, sizeof(state_t));

	s->sp = RAMTOP;
	s->pc = (unsigned) sys_callback;
	s->cpsr = STATUS_SYS_MODE;
	s->cpsr = STATUS_DISABLE_INT(s->cpsr);
	s->CP15_Control = CP15_DISABLE_VM(s->CP15_Control);
}
#endif

void sys_callback() {
	state_t *to_restore = (state_t*) SYSBK_OLDAREA;

	bka_strncpy(message, "Hello! This is sys_callback()!.\n", 129);

#ifdef BKA_ARCH_UMPS
	to_restore->pc_epc += WS;
#endif

	LDST(to_restore);
}
