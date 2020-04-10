#include "callback.h"
#include "string.h"


void bka_na_init(state_t *s, nac_t c) {
	/* initialize all of s to 0 */
	bka_memset(s, 0, sizeof(*s));

#ifdef BKA_ARCH_UMPS
	s->pc_epc = (unsigned) c;
	s->reg_sp = BKA_RAMTOP;
#elif defined(BKA_ARCH_UARM)
	s->pc = (unsigned) c;
	s->sp = BKA_RAMTOP;
	s->cpsr |= STATUS_SYS_MODE;
	s->cpsr |= STATUS_ALL_INT_DISABLE(s->cpsr);
#endif
}

void bka_na_exit(unsigned new_area) {
#ifdef BKA_ARCH_UMPS
	state_t *old_state = (state_t *) (new_area - RRF_FS);

	old_state->pc_epc += WS;
#elif defined(BKA_ARCH_UARM)
	state_t *old_state = (state_t *) (new_area - STATE_T_SIZE);

	/* TODO Is this actually needed? */
    if (new_area == SYSBK_NEWAREA)
    	old_state->pc += WS;
    else if (new_area == INT_NEWAREA)
    	old_state->pc -= WS;
#endif

	LDST(old_state);
}
