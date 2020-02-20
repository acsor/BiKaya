#ifndef BKA_MATH_H
#define BKA_MATH_H


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
 * @return Biggest integer @c n such that <tt>base ^ n &lt;= x</tt>.
 */
int bka_log(int base, int x);


#endif
