#ifndef BKA_MATH_H
#define BKA_MATH_H

/* TODO Redefine as the least possible negative number. */
#define BKA_NINFTY -1


int bka_abs(int x);
/**
 * @return Integer minimum value of <tt>{a, b}</tt>.
 */
int bka_min(int a, int b);
/**
 * @return Integer maximum value of <tt>{a, b}</tt>.
 */
int bka_max(int a, int b);
/**
 * @return Biggest integer @c n such that <tt>base ^ n &lt;= x</tt>. As a
 * special case, if <tt>x == 0</tt>, @c BKA_NINFTY is returned.
 */
int bka_log(int base, unsigned x);


#endif
