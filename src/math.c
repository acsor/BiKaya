#include "math.h"


int bk_abs(int x) {
	return x >= 0 ? x: -x;
}

int bk_min(int a, int b) {
	return a <= b ? a: b;
}

int bk_max(int a, int b) {
	return a >= b ? a: b;
}

int bk_log(int base, unsigned x) {
	int exp = 0;

	if (x == 0)
		return BK_NINFTY;

	while (x >= base) {
		x /= base;
		exp++;
	}

	return exp;
}
