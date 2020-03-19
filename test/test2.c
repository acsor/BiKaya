#include "io.h"
#include "sys.h"
#include "test2_aux.h"


int main () {
	termreg_t * const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	bka_term_puts(term0, "Nothing to show yet.\n", NULL);

	return 0;
}