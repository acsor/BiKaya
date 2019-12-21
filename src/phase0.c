#include "system.h"
#include "termprint.h"


int main(void) {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);
	bka_term_puts(term0, "Hello, world!");

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();

    return 0;
}
