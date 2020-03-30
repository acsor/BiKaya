#include "callback.h"
#include "string.h"


void bka_na_init(state_t *s, nac_t c) {
#ifdef BKA_ARCH_UMPS
	s->pc_epc = (unsigned) c;
	s->reg_sp = BKA_RAMTOP;
	
	// initialize the status register
	bka_memset(&s->status, 0, sizeof(s->status));
#elif defined(BKA_ARCH_UARM)
	s->pc = (unsigned) c;
	s->sp = BKA_RAMTOP;
	
	// initialize the status register
	// TODO How do we know interrupts are disabled?
	bka_memset(&s->cpsr, 0, sizeof(s->cpsr));
	bka_memset(&s->CP15_Control, 0, sizeof(s->CP15_Control));
	s->cpsr |= STATUS_SYS_MODE;
#endif
}
