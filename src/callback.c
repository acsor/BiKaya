#include "callback.h"
#include "string.h"


void bka_na_init(state_t *s, nac_t c) {
	// initialize all of s to 0
	bka_memset(s, 0, sizeof(*s));

#ifdef BKA_ARCH_UMPS
	s->pc_epc = (unsigned) c;
	s->reg_sp = BKA_RAMTOP;
#elif defined(BKA_ARCH_UARM)
	s->pc = (unsigned) c;
	s->sp = BKA_RAMTOP;
	// interrupt disabilitati perchè l'ottavo bit è stato impostato a 0
	s->cpsr |= STATUS_SYS_MODE;
#endif
}

void bka_na_exit(unsigned new_area) {
#ifdef BKA_ARCH_UMPS
	state_t *old_state = (state_t *) (new_area - RRF_FS);

	old_state->pc_epc += WS;
#elif defined(BKA_ARCH_UARM)
	state_t *old_state = (state_t *) (new_area - STATE_T_SIZE);
#endif

	LDST(old_state);
}
