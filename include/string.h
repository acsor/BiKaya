#ifndef BKA_STRING_H
#define BKA_STRING_H

#include "sys.h"


/**
 * @param src String representation of integer number
 * @param base Base in which the output should be translated
 * @return The integer value of @c src in base @base, or @c BKA_E_INVARG
 * if either @c src contained invalid digits or @c base was < 2.
 */
long int bka_atoi(char const *src, unsigned base);
/**
 * Like bka_atoi(), but in base 10.
 */
long int bka_atoi10(char const *src);
/**
 * Converts @n in string format, storing it into @c dest for a maximum of @c
 * length characters written.
 * @return @c BKA_E_GEN if an error occurred, @c BKA_E_OK otherwise.
 */
int bka_itoa(int n, char *dest, unsigned length);

/**
 * @return Number of characters in @c str, or an appropriate @c BKA_E_X error
 * if something goes wrong.
 */
int bka_strlen(char const *str);
/**
 * Copies at most @length characters from @c src to @c dest, stopping early if
 * the terminating @c '\0' character is encountered.
 * @return Number of characters effectively copied, or a @c BKA_E_X error
 * otherwise.
 */
int bka_strncpy(char *dest, char const *src, unsigned length);
/**
 * Copies @c src into @c dest, until the terminating @c '\0' character is
 * encountered.
 * @return Number of characters effectively copied, including the @c '\0'
 * character, or a @c BKA_E_X error otherwise.
 */
int bka_strcpy(char *dest, char const *src);


#endif
