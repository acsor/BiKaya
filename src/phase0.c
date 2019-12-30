#include "sys.h"
#include "io.h"


int main () {
	unsigned const buffsize = 256;
	char buff[buffsize];

	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);
	dtpreg_t *print0 = (dtpreg_t*) DEV_REG_ADDR(IL_PRINTER, 0);

	*buff = '\0';
	bka_term_puts(
		term0,
		"Please, enter some text to redirect to the printer 0\n> "
	);

	if (bka_term_recvs(term0, buff, buffsize) == BKA_E_GEN) {
		bka_term_puts(term0, "Errors occurred");
	} else {
		bka_print_puts(print0, buff);
		bka_term_puts(term0, "Printed successfully to printer 0");
	}

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();

    return 0;
}
