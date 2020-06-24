#include "io.h"
#include "stdarg.h"

/**
 * Generates a terminal device transmission command with argument @c c.
 */
#define TERM_TRANSM_COMMAND(c)		((c << 8) | TERM_CMD_TRANSM)
/**
 * @return Terminal device transmit status of @c term.
 */
#define TERM_TRANSM_STATUS(term)	(term->transm_status & 0xFF)

/**
 * @return Terminal device receive status of @c term.
 */
#define TERM_RECV_STATUS(term)	(term->recv_status & 0xFF)
/**
 * @return The received character from a terminal device receive register.
 */
#define TERM_RECV_CHAR(term)	((term->recv_status & 0xFF00) >> 8)


static int io_dev_to_sem[(N_EXT_IL - 1) * N_DEV_PER_IL];
static int io_termdev_to_sem[2 * N_DEV_PER_IL];


/**
 * Auxiliary function for @c bka_term_puts.
 * @return Number of characters from @c str written to @c term.
 * @see bka_term_puts
 */
static int bka_term_puts_aux(termreg_t *term, char const *str);

/**
 * Issues a write of character @c c to terminal @c term.
 * @return @c BKA_E_GEN in case of errors, @c BKA_E_OK otherwise.
 */
static int bka_term_putchar(termreg_t *term, char c);

/**
 * Issues a write of character @c to printer device @c p.
 * @return @c BKA_E_GEN in case of errors, @c BKA_E_OK otherwise.
 */
static int bka_print_putchar(dtpreg_t *p, char c);


int bka_term_puts(termreg_t *term, ...) {
	int written = 0;
	char *to_print = NULL;
	va_list ap;

	va_start(ap, term);

	while ((to_print = va_arg(ap, char*)) != NULL) {
		bka_term_puts_aux(term, to_print);
		written++;
	}

	va_end(ap);

	return written;
}

int bka_term_recvc(termreg_t *term) {
	unsigned status = TERM_RECV_STATUS(term);
	char read;

	if (status != TERM_ST_READY && status != TERM_ST_RECEIVED)
		return BKA_E_GEN;

	term->recv_command = TERM_CMD_RECV;

	while ((status = TERM_RECV_STATUS(term)) == TERM_ST_BUSY)
		;

	read = TERM_RECV_CHAR(term);
	term->recv_command = TERM_CMD_ACK;

	return status == TERM_ST_RECEIVED ? read: BKA_E_GEN;
}

int bka_term_recvs(termreg_t *term, char *dest, unsigned length) {
	int in = -1;

	while (length > 1 && in != '\n') {
		if ((in = bka_term_recvc(term)) == BKA_E_GEN)
			return BKA_E_GEN;

		*dest = in;
		dest++;
		length--;
	}

	*dest = '\0';

	return BKA_E_OK;
}


int bka_print_puts(dtpreg_t *dev, char const *str) {
	int written;

	for (written = 0; *str; written++, str++) {
		if (bka_print_putchar(dev, *str) != BKA_E_OK)
			return BKA_E_GEN;
	}

	return written;
}


/* TODO Move auxiliary functions to the bottom of the file. */
int bka_term_puts_aux(termreg_t *term, char const *str) {
	int written;

	for (written = 0; *str; written++, str++) {
		if (bka_term_putchar(term, *str) != BKA_E_OK)
			return BKA_E_GEN;
	}

	return written;
}

static int bka_term_putchar(termreg_t *term, char c) {
    unsigned stat = TERM_TRANSM_STATUS(term);

    if (stat != TERM_ST_READY && stat != TERM_ST_TRANSMITTED)
        return BKA_E_GEN;

    term->transm_command = TERM_TRANSM_COMMAND(c);

    while ((stat = TERM_TRANSM_STATUS(term)) == TERM_ST_BUSY)
        ;

    term->transm_command = TERM_CMD_ACK;

    return stat != TERM_ST_TRANSMITTED ? BKA_E_GEN: BKA_E_OK;
}

static int bka_print_putchar(dtpreg_t *p, char c) {
	unsigned int status = p->status & PRINT_STATUS_MASK;

	/* TODO Can we expand the set of allowable statuses? */
	if (status != PRINT_ST_READY)
		return BKA_E_GEN;

	p->data0 = c;
	p->command = PRINT_CMD_PRINTC;

	while ((status = p->status & PRINT_STATUS_MASK) == PRINT_ST_BUSY)
		;

	p->command = PRINT_CMD_ACK;

	return status != PRINT_ST_READY ? BKA_E_GEN: BKA_E_OK;
}


semd_t* bka_dev_sem_get(void *dev, unsigned subdevice) {
	semd_t *out;
	unsigned line = bka_dev_line(dev), instance = bka_dev_instance(dev);
	int *semkey;

	if (line == IL_TERMINAL)
		semkey = io_termdev_to_sem + N_DEV_PER_IL * subdevice + instance;
	else
		semkey = io_dev_to_sem + N_DEV_PER_IL * EXT_IL_INDEX(line) + instance;

	if (!(out = bka_sem_get(semkey)))
		out = bka_sem_alloc(semkey);

	return out;
}

unsigned bka_dev_line(void *dev) {
	/* TODO Implement for uARM too. */
	/* TODO Turn this into a macro. */
	dtpreg_t *first = (dtpreg_t *) DEV_REG_ADDR(IL_DISK, 0);

	return (((dtpreg_t *) dev) - first) / N_DEV_PER_IL;
}

unsigned bka_dev_instance(void *dev) {
	/* TODO Implement for uARM too. */
	/* TODO Turn this into a macro. */
	unsigned line = bka_dev_line(dev);
	dtpreg_t *first = (dtpreg_t *) DEV_REG_ADDR(line, 0);

	return ((dtpreg_t *) dev) - first;
}
