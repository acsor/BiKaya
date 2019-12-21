#include "sys.h"

#define ST_READY           1
#define ST_BUSY            3
#define ST_TRANSMITTED     5

#define CMD_ACK            1
#define CMD_TRANSMIT       2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF

/**
 * TODO Document.
 * @param tp
 * @return
 */
static unsigned int bka_term_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

/**
 * Issues a write of character @c c to terminal @c term.
 * @return @c BKA_E_GEN in case of errors, @c BKA_E_OK otherwise.
 */
static int bka_term_putchar(termreg_t *term, char c) {
    unsigned int stat;

    stat = bka_term_status(term);
    if (stat != ST_READY && stat != ST_TRANSMITTED)
        return BKA_E_GEN;

    term->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    while ((stat = bka_term_status(term)) == ST_BUSY)
        ;

    term->transm_command = CMD_ACK;

    if (stat != ST_TRANSMITTED)
        return BKA_E_GEN;
    else
        return BKA_E_OK;
}


int bka_term_puts(termreg_t *term, char const *str) {
	unsigned written;

	for (written = 0; *str; written++, str++) {
		if (bka_term_putchar(term, *str) != BKA_E_OK)
			return BKA_E_GEN;
	}

	return written;
}
