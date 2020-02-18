#ifndef BKA_IO
#define BKA_IO

#include "sys.h"

/**
 * @param term Terminal register to write to
 * @param str  Null-terminated string to write into @c term
 * @return @c BKA_E_GEN if errors occurred, count of written characters
 * otherwise.
 */
int bka_term_puts(termreg_t *term, char const *str);
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
 *
 * @param dev
 * @param str Null-terminated string to write into @c dev
 * @return @c BKA_E_GEN if errors occurred, count of written characters
 * otherwise.
 */
int bka_print_puts(dtpreg_t *dev, char const *str);


#endif