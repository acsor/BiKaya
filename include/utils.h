#ifndef BK_UTILS_H
#define BK_UTILS_H

/**
 * Macro returning the dimension of an array of any type.
 */
#define BK_LENGTH(array, type) sizeof(array) / sizeof(type)
/**
 * @return Number of digits of @c n, interpreted as a base 10 number.
 */
int bk_digits(int n);


#endif