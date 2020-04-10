#include "callback.h"
#include "string.h"


void bka_na_init(state_t *s, sec_t c) {
	/* initialize all of s to 0 */
	bka_memset(s, 0, sizeof(*s));

#ifdef BKA_ARCH_UMPS
	s->pc_epc = (unsigned) c;
	s->reg_sp = BKA_RAMTOP;
#elif defined(BKA_ARCH_UARM)
	s->pc = (unsigned) c;
	s->sp = BKA_RAMTOP;
	s->cpsr = STATUS_SYS_MODE;
	s->cpsr = STATUS_ALL_INT_DISABLE(s->cpsr);
	s->CP15_Control = CP15_DISABLE_VM(s->CP15_Control);
#endif
}

void bka_na_enter(unsigned new_area) {
#ifdef BKA_ARCH_UMPS
	if (new_area == SYSBK_NEWAREA) {
		state_t *old_area = (state_t *) SYSBK_OLDAREA;

		old_area->pc_epc -= WS;
	}
#elif defined(BKA_ARCH_UARM)
	state_t *old_area = (state_t *) (new_area - STATE_T_SIZE);

	old_area->pc -= WS;
#endif
}

void bka_na_exit(unsigned new_area) {
#ifdef BKA_ARCH_UMPS
	state_t *old_state = (state_t *) (new_area - RRF_FS);
#elif defined(BKA_ARCH_UARM)
	state_t *old_state = (state_t *) (new_area - STATE_T_SIZE);
#endif

	LDST(old_state);
}
