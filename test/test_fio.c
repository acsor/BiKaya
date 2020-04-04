#include "io.h"
#include "string.h"
#include "sys.h"


int main () {
	termreg_t *term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	bka_printf(term0, "ds",12, "ciaone");
	HALT();

    return 0;
}
