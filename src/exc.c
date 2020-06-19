#include "exc.h"
#include "pcb.h"
#include "sched.h"
#include "string.h"
#include "time.h"


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
	bka_kernel_on_enter();

#ifdef BKA_ARCH_UMPS
	if (new_area == SYSBK_NEWAREA) {
		state_t *oa = (state_t *) SYSBK_OLDAREA;

		if (CAUSE_GET_EXCCODE(BKA_STATE_CAUSE(oa)) == EXC_SYS)
			oa->pc_epc += WS;
	}
#elif defined(BKA_ARCH_UARM)
	/* TODO Decrementing the old area word for uARM might be unnecessary. */
	if (new_area == INT_NEWAREA || new_area == SYSBK_NEWAREA) {
		state_t *old_area = (state_t *) (new_area - STATE_T_SIZE);

		old_area->pc -= WS;
	}
#endif
}

void bka_na_exit(unsigned new_area) {
#ifdef BKA_ARCH_UMPS
	state_t *old_state = (state_t *) (new_area - RRF_FS);
#elif defined(BKA_ARCH_UARM)
	state_t *old_state = (state_t *) (new_area - STATE_T_SIZE);
#endif

	bka_kernel_on_exit();

	LDST(old_state);
}


void bka_sp_enter(unsigned type) {
	state_t *old_area_src[] = {
		(state_t *) SYSBK_OLDAREA, (state_t *) TLB_OLDAREA,
		(state_t *) PGMTRAP_OLDAREA,
	};

	*bka_sched_curr->sp_areas[type][0] = *(old_area_src[type]);
}

void bka_sp_exit(unsigned type) {
}


void bka_kernel_on_enter() {
	bka_pcb_time_push(bka_sched_curr, 0);
}

void bka_kernel_on_exit() {
	bka_pcb_time_pop(bka_sched_curr);
}
