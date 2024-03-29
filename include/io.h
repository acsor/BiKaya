#ifndef BK_IO
#define BK_IO

#include "arch.h"
#include "sem.h"


#define DEV_CMD_RESET	0
#define DEV_CMD_ACK		1
/* TODO Complete generic device command constants. */

#define BK_PRINT(print_no)	(dtpreg_t *) (DEV_REG_ADDR(IL_PRINTER, print_no))

#define PRINT_ST_NOTINST	0
#define PRINT_ST_READY		1
#define PRINT_ST_IOCE		2
#define PRINT_ST_BUSY		3
#define PRINT_ST_PRINTE		4

#define PRINT_CMD_RESET		0
#define PRINT_CMD_ACK       1
#define PRINT_CMD_PRINTC    2

/**
 * Printer device status mask.
 */
#define PRINT_ST_MASK	0xFF

#define BK_TERM(term_no)	(termreg_t *) (DEV_REG_ADDR(IL_TERMINAL, term_no))

#define TERM_ST_NOTINST		0
#define TERM_ST_READY       1
#define TERM_ST_IOCE		2
#define TERM_ST_BUSY        3
#define TERM_ST_TRANSME		4
#define TERM_ST_RECVE		4
#define TERM_ST_TRANSMITTED	5
#define TERM_ST_RECEIVED	5

/**
 * Terminal device status mask.
 */
#define TERM_ST_MASK		0xFF

#define TERM_CMD_RESET		0
#define TERM_CMD_ACK		1
#define TERM_CMD_TRANSM		2
#define TERM_CMD_RECV		2


/**
 * Print to the terminal device @c term until a @c NULL is met in the
 * variable-argument list of strings.
 * @param term Terminal register to write to
 * @return @c BK_E_GEN if errors occurred, count of written strings otherwise.
 */
int bk_term_puts(termreg_t *term, ...);
#define bk_term_puts2(term_no, ...) bk_term_puts(BK_TERM(term_no), __VA_ARGS__)
/**
 * Reads a single character from @c term.
 * @return @c BK_E_GEN if errors occurred, number of read characters
 * otherwise.
 */
int bk_term_recvc(termreg_t *term);
#define bk_term_recvc2(term_no) bk_term_recvc(BK_TERM(term_no))
/**
 * Reads at most <code>length - 1</code> characters from @c term into @c
 * dest, stopping earlier if a newline character is met. A @c '\0'
 * terminating character is stored at the end of the string (which is why
 * only <code>length - 1</code> effective characters are stored).
 * @param term Terminal device to read from
 * @param dest Destination string to write to
 * @param length Maximum number of characters to read, @c '\0' included
 * @return @c BK_E_GEN in case of errors, @c BK_E_OK otherwise.
 */
int bk_term_recvs(termreg_t *term, char *dest, unsigned length);
#define bk_term_recvs2(term_no, dest, length) bk_term_recvs(BK_TERM (term_no), dest, length)


/**
 * @param dev
 * @param str Null-terminated string to write into @c dev
 * @return @c BK_E_GEN if errors occurred, count of written characters
 * otherwise.
 */
 /* TODO Turn this function into variadic. */
int bk_print_puts(dtpreg_t *dev, char const *str);
#define bk_print_puts2(dev_no, str) bk_print_puts(BK_PRINT(dev_no), str)


/**
 * Obtains a semaphore association for the given device register @c dev.
 * Enough sempahore (integer) keys are preallocated for each device, so that
 * cases where an insufficient supply is reached cannot be met.
 * @param dev Device to get the associated semaphore for.
 * @param subdevice Relevant for terminal devices only; equals @c 0 when
 * referring to the transmission subdevice, @c 1 when referring to the
 * reception one.
 * @return The semaphore key associated to the device @c dev.
 */
int* bk_dev_sem_get(void *dev, unsigned subdevice);
/**
 * @return The next pending interrupted device register, in increasing order
 * from lower interrupt lines to higher ones. Internal devices are not
 * considered, so that only device register pointers from disks to terminals
 * can be returned by this function.
 * <b>Note</b>: for terminals, it is sufficient that either the transmitting
 * side or the receiving side has a pending interrupt for the function to
 * return the corresponding terminal register.
 */
void* bk_dev_next_pending();
/**
 * Acknowledges a pending interrupt for the device @c device on interrupt
 * line @c line, optionally identified by @c subdevice (appropriate
 * for terminal devices only).
 * @param line The interrupt line
 * @param device The device number
 * @param subdevice The subdevice number. This parameter is only read when
 * dealing with terminal devices; if <tt>subdevice == 0</tt>, the receive
 * subdevice interrupt is acknowledged only, if <tt>subdevice == 1</tt> the
 * transmission subdevice instead.
 * @see bk_dev_ack2
 */
void bk_dev_ack(unsigned line, unsigned device, unsigned subdevice);
/**
 * An alternative way of invoking @c bk_dev_ack().
 * @see bk_dev_ack
 */
#define bk_dev_ack2(dev, subdev)	bk_dev_ack(bk_dev_line(dev), bk_dev_instance(dev), subdev)
/**
 * @return The interrupt line number the device register @c dev belongs to.
 */
unsigned bk_dev_line(void *dev);
/**
 * @return The "instance" number within the appropriate interrupt line the
 * device register @c dev belongs to.
 */
unsigned bk_dev_instance(void *dev);


#endif