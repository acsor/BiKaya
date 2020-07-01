#include "arch.h"
#include "io.h"


int main () {
	unsigned const buffsize = 256;
	char buff[buffsize];

	*buff = '\0';
	bk_term_puts2(
		0, "Please, enter some text to redirect to the printer 0\n> ", NULL
	);

	if (bk_term_recvs2(0, buff, buffsize) == BK_E_GEN) {
		bk_term_puts2(0, "Errors occurred", NULL);
	} else {
		bk_print_puts2(0, buff);
		bk_term_puts2(0, "Printed successfully to printer 0", NULL);
	}

    /* Go to sleep indefinitely */
    while (1) 
        WAIT();

    return 0;
}
