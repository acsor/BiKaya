#include "io.h"
#include "string.h"
#include "sys.h"


int main () {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	bka_printf(term0, "Hello, my name is %s and I'm %s.\n", "%s", "%d");
	bka_printf(term0, "Hello, my name is %s and I'm %d.\n", "Alexander", 19);
	HALT();

    return 0;
}
