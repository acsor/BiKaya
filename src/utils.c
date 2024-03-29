#include "math.h"
#include "utils.h"


int bk_digits(int n) {
	if (n < 0)
		n *= -1;
	else if (n == 0)
		return 1;

	return bk_log(10, n) + 1;
}
