#include "callback.h"


void bka_na_init(state_t *s, nac_t c) {
	
	#ifdef BKA_ARCH_UMPS
	s -> pc_epc = c;
	s -> reg_sp = BKA_RAMTOP;
	
	//initialize the status register
	bka_memset(s -> status, 0, sizeof(unsigned int));
	
	#elif defined(BKA_ARCH_UARM)
	s -> pc = c;
	s -> sp = BKA_RAMTOP;
	
	//initialize the status register
	bka_memset(s -> cpsr, 0, sizeof(unsigned int));
	bka_memset(s -> CP15_Control, 0, sizeof(unsigned int));
	s -> cpsr |= 0x1F;
	
	#endif
}



