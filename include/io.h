#ifndef BKA_IO
#define BKA_IO

#include "arch.h"
#include "sem.h"


#define DEV_CMD_RESET	0
#define DEV_CMD_ACK		1
/* TODO Complete generic device command constants. */

#define BKA_PRINT(print_no)	(dtpreg_t *) (DEV_REG_ADDR(IL_PRINTER, print_no))

#define PRINT_ST_NOTINST	0
#define PRINT_ST_READY		1
#define PRINT_ST_IOCE		2
#define PRINT_ST_BUSY		3
#define PRINT_ST_PRINTE		4

#define PRINT_CMD_RESET		0
#define PRINT_CMD_ACK       1
#define PRINT_CMD_PRINTC    2

#define PRINT_STATUS_MASK	0xFF

#define BKA_TERM(term_no)	(termreg_t *) (DEV_REG_ADDR(IL_TERMINAL, term_no))

#define TERM_ST_NOTINST		0
#define TERM_ST_READY       1
#define TERM_ST_IOCE		2
#define TERM_ST_BUSY        3
#define TERM_ST_TRANSME		4
#define TERM_ST_RECVE		4
#define TERM_ST_TRANSMITTED	5
#define TERM_ST_RECEIVED	5

#define TERM_CMD_RESET		0
#define TERM_CMD_ACK		1
#define TERM_CMD_TRANSM		2
#define TERM_CMD_RECV		2


/**
 * Print to the terminal device @c term until a @c NULL is met in the
 * variable-argument list of strings.
 * @param term Terminal register to write to
 * @return @c BKA_E_GEN if errors occurred, count of written strings otherwise.
 */
int bka_term_puts(termreg_t *term, ...);
#define bka_term_puts2(term_no, ...) bka_term_puts(BKA_TERM(term_no), __VA_ARGS__)
/**
 * Reads a single character from @c term.
 * @return @c BKA_E_GEN if errors occurred, number of read characters
 * otherwise.
 */
int bka_term_recvc(termreg_t *term);
#define bka_term_recvc2(term_no) bka_term_recvc(BKA_TERM(term_no))
/**
 * Reads at most <code>length - 1</code> characters from @c term into @c
 * dest, stopping earlier if a newline character is met. A @c '\0'
 * terminating character is stored at the end of the string (which is why
 * only <code>length - 1</code> effective characters are stored).
 * @param term Terminal device to read from
 * @param dest Destination string to write to
 * @param length Maximum number of characters to read, @c '\0' included
 * @return @c BKA_E_GEN in case of errors, @c BKA_E_OK otherwise.
 */
int bka_term_recvs(termreg_t *term, char *dest, unsigned length);
#define bka_term_recvs2(term_no, dest, length) bka_term_recvs(BKA_TERM (term_no), dest, length)


/**
 * @param dev
 * @param str Null-terminated string to write into @c dev
 * @return @c BKA_E_GEN if errors occurred, count of written characters
 * otherwise.
 */
 /* TODO Turn this function into variadic. */
int bka_print_puts(dtpreg_t *dev, char const *str);
#define bka_print_puts2(dev_no, str) bka_print_puts(BKA_PRINT(dev_no), str)


/**
 * Obtains a semaphore association for the given device register @c dev. If
 * one such association does not exist a new one, if possible, is created;
 * otherwise, the already existing one is returned.
 * @param dev Device to get the associated semaphore for.
 * @param subdevice Relevant for terminal devices only; equals @c 0 when
 * referring to the transmission subdevice, @c 1 when referring to the
 * reception one.
 * @return A semaphore associated to the device @c dev, @c NULL in case the
 * free semaphore list (FSL) is empty.
 */
semd_t* bka_dev_sem_get(void *dev, unsigned subdevice);
/**
 * @return The interrupt line number the device register @c dev belongs to.
 */
unsigned bka_dev_line(void *dev);
/**
 * @return The "instance" number within the appropriate interrupt line the
 * device register @c dev belongs to.
 */
unsigned bka_dev_instance(void *dev);


#endif