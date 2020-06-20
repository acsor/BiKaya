#include "arch.h"
#include "io.h"


int main () {
	unsigned const buffsize = 256;
	char buff[buffsize];

	*buff = '\0';
	bka_term_puts2(
		0, "Please, enter some text to redirect to the printer 0\n> ", NULL
	);

	if (bka_term_recvs2(0, buff, buffsize) == BKA_E_GEN) {
		bka_term_puts2(0, "Errors occurred", NULL);
	} else {
		bka_print_puts2(0, buff);
		bka_term_puts2(0, "Printed successfully to printer 0", NULL);
	}

    /* Go to sleep indefinetely */
    while (1)
        WAIT();

    return 0;
}
