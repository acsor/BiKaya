#ifndef BK_MATH_H
#define BK_MATH_H

/* TODO Redefine as the least possible negative number. */
#define BK_NINFTY -1


int bk_abs(int x);
/**
 * @return Integer minimum value of <tt>{a, b}</tt>.
 */
int bk_min(int a, int b);
/**
 * @return Integer maximum value of <tt>{a, b}</tt>.
 */
int bk_max(int a, int b);
/**
 * @return Biggest integer @c n such that <tt>base ^ n &lt;= x</tt>. As a
 * special case, if <tt>x == 0</tt>, @c BK_NINFTY is returned.
 */
int bk_log(int base, unsigned x);


#endif
