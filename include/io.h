#ifndef BKA_IO
#define BKA_IO

#include "arch.h"
#include "sem.h"


/**
 * Print to the terminal device @c term until a @c NULL is met in the
 * variable-argument list of strings.
 * @param term Terminal register to write to
 * @return @c BKA_E_GEN if errors occurred, count of written strings otherwise.
 */
int bka_term_puts(termreg_t *term, ...);
/**
 * Reads a single character from @c term.
 * @return @c BKA_E_GEN if errors occurred, number of read characters
 * otherwise.
 */
int bka_term_recvc(termreg_t *term);
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


/**
 * @param dev
 * @param str Null-terminated string to write into @c dev
 * @return @c BKA_E_GEN if errors occurred, count of written characters
 * otherwise.
 */
 /* TODO Turn this function into variadic. */
int bka_print_puts(dtpreg_t *dev, char const *str);


/**
 * Obtains a semaphore association for the given device register @c dev. If
 * one such association does not exist a new one, if possible, is created;
 * otherwise, the already existing one is returned.
 *
 * @b Note: the actual type of the device register is irrelevant. In order
 * to identify one such device, its physical memory address is sufficient.
 *
 * @param dev Device to get the associated semaphore for.
 * @param subdevice @c dev subdevice, mostly relevant for terminals only
 * @return A semaphore associated to the device @c dev, @c NULL in case the
 * free semaphore list (FSL) is empty.
 */
semd_t* bka_dev_sem_get(unsigned *dev, unsigned subdevice);
/**
 * @return The interrupt line number the device register @c dev belongs to.
 */
unsigned bka_dev_line(unsigned *dev);
/**
 * @return The "instance" number within the appropriate interrupt line the
 * device register @c dev belongs to.
 */
unsigned bka_dev_instance(unsigned *dev);


#endif