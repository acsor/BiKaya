#include "callback.h"
#include "string.h"


void bka_na_init(state_t *s, nac_t c) {
#ifdef BKA_ARCH_UMPS
	// initialize all registers
	bka_memset(&s->entry_hi, 0, sizeof((unsigned int)*37));//state_t fields initialized to 0
	
	s->pc_epc = (unsigned) c;
	s->reg_sp = BKA_RAMTOP;
	
	
#elif defined(BKA_ARCH_UARM)
	// initialize all registers
	bka_memset(&s -> a1, 0, (sizeof(unsigned int)*22));//state_t fields initialized to 0
	
	s->pc = (unsigned) c;
	s->sp = BKA_RAMTOP;
	//interrupts disabilitati perchè l'ottavo bit è stato impostato a 0
	s->cpsr |= STATUS_SYS_MODE;
#endif
}
