#ifndef __TERMPRINT_H__
#define __TERMPRINT_H__


/**
 * @param term Terminal register to write to
 * @param str  Series of characters to send into @c term
 * @return @c E_GENERIC if errors occurred, count of written characters
 * otherwise.
 */
int bka_term_puts(termreg_t *term, char const *str);


#endif