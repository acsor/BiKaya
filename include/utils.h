#ifndef BKA_UTILS_H
#define BKA_UTILS_H

/**
 * Macro returning the dimension of an array of any type.
 */
#define BKA_LENGTH(array, type) sizeof(array) / sizeof(type)
/**
 * @return Number of digits of @c n, interpreted as a base 10 number.
 */
int bka_digits(int n);


#endif