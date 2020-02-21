#include "math.h"


int bka_abs(int x) {
	return x >= 0 ? x: -x;
}

int bka_min(int a, int b) {
	return a <= b ? a: b;
}

int bka_max(int a, int b) {
	return a >= b ? a: b;
}

int bka_log(int base, unsigned x) {
	int exp = 0;

	if (x == 0)
		return BKA_NINFTY;

	while (x >= base) {
		x /= base;
		exp++;
	}

	return exp;
}
