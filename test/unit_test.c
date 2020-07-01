#include "io.h"
#include "arch.h"
#include "math.h"
#include "string.h"
#include "utils.h"


/**
 * Type of a test function, taking as arguments a pointer to void, a pointer
 * to an error string and its size.
 */
typedef int (*test_function)(void *const, char[], int);

typedef struct {
	char *name;
	test_function f;
} test_t;

int test_bk_atoi(void *data, char* errmsg, int errdim);
int test_bk_itoa(void *data, char* errmsg, int errdim);
int test_bk_strlen(void *data, char* errmsg, int errdim);
int test_bk_strncpy(void *data, char* errmsg, int errdim);
int test_bk_strcpy(void *data, char* errmsg, int errdim);
int test_bk_strcmp(void *data, char* errmsg, int errdim);
int test_bk_memset(void *data, char* errmsg, int errdim);

int test_bk_log(void *data, char* errmsg, int errdim);

int test_bk_dev_line(void *data, char* errmsg, int errdim);


int main (int argc, char *argv[]) {
	test_t tests[] = {
		{"test_bk_atoi", test_bk_atoi}, {"test_bk_itoa", test_bk_itoa},
		{"test_bk_strlen", test_bk_strlen},
		{"test_bk_strncpy", test_bk_strncpy},
		{"test_bk_strcpy", test_bk_strcpy},
		{"test_bk_strcmp", test_bk_strcmp},
		{"test_bk_memset", test_bk_memset},

		{"test_bk_log", test_bk_log},

		{"test_bk_dev_line", test_bk_dev_line},
	};
	test_function f;

	unsigned const buffsize = 128;
	char errmsg[buffsize];
	int const tests_no = BK_LENGTH(tests, test_t);
	int i, failed_no = 0;

	for (i = 0; i < tests_no; i++) {
		errmsg[0] = '\0';
		bk_term_puts2(0, "Testing ", tests[i].name, "()... ", NULL);
		f = tests[i].f;

		/* If the test fails... */
		if (f(NULL, errmsg, buffsize)) {
			if (errmsg[0] == '\0')
				bk_strncpy(errmsg, "UNKNOWN", buffsize);

			bk_term_puts2(0, "FAILED.\n\tREASON: ", errmsg, "\n", NULL);
			failed_no++;
		} else {
			bk_term_puts2(0, "OK.\n", NULL);
		}
	}

	/* printf( */
	/* 		"\nRUN %lu TESTS (%lu OK, %lu FAILED).\n", */
	/* 		tests_no, tests_no - failed_no, failed_no */
	/* ); */
	bk_term_puts2(0, "\nALL TESTS RUN.\n", NULL);

	return 0;
}


int test_bk_atoi(void *data, char *errmsg, int errdim) {
	char *input[] = {"0", "10", "123", "789"};
	int base[] = {10, 10, 10, 10};
	int expected[] = {0, 10, 123, 789};

	int const n = BK_LENGTH(base, int);
	int i;

	for (i = 0; i < n; i++) {
		if (bk_atoi(input[i], base[i]) != expected[i])
			return 1;
	}

	return 0;
}

int test_bk_itoa(void *data, char *errmsg, int errdim) {
	unsigned const buffsize = 6;
	char buff[buffsize];

	int inputs[] = {-12, -1, 0, 1, 10, 42, 129, 123456};
	char *expected[] = {"-12", "-1", "0", "1", "10", "42", "129", "12345"};
	unsigned const n = BK_LENGTH(inputs, int);
	int i;

	for (i = 0; i < n; i++) {
		bk_itoa(inputs[i], buff, buffsize);

		if (bk_strcmp(expected[i], buff)) {
			bk_strncpy(errmsg, buff, errdim);

			return 1;
		}
	}

	return 0;
}

int test_bk_strlen(void *data, char* errmsg, int errdim) {
	char *inputs[] = {"", "1", "one", "fortytwo"};
	unsigned const expected[] = {0, 1, 3, 8};
	int i;

	for (i = 0; i < BK_LENGTH(inputs, char*); i++) {
		if (bk_strlen(inputs[i]) != expected[i])
			return 1;
	}

	return 0;
}

int test_bk_strncpy(void *data, char* errmsg, int errdim) {
	unsigned const buffsize = 64;
	char src[buffsize], dest[buffsize];
	char *inputs[] = {"", "1234567890", "fortytwo", "insanity"};
	int i;

	for (i = 0; i < BK_LENGTH(inputs, char*); i++) {
		bk_strncpy(dest, src, buffsize);

		if (bk_strcmp(src, dest))
			return 1;
	}

	return 0;
}

int test_bk_strcpy(void *data, char* errmsg, int errdim) {
	unsigned const buffsize = 64;
	char src[buffsize], dest[buffsize];
	char *inputs[] = {"", "1234567890", "fortytwo", "insanity"};
	int i;

	for (i = 0; i < BK_LENGTH(inputs, char*); i++) {
		bk_strcpy(dest, src);

		if (bk_strcmp(src, dest))
			return 1;
	}

	return 0;
}

int test_bk_strcmp(void *data, char* errmsg, int errdim) {
	char *inputs[] = {
		/* Equal strings */
		"", "",
		"1234567890", "1234567890",
		"fortytwo", "fortytwo",
		"AAABBBCCCDDDEEEFFF","AAABBBCCCDDDEEEFFF",

		/* Unequal strings */
		"", " ",
		"12345 67890", "1234567890",
		"fortytwx", "fortytwo",
		"XAABBBCCCDDDEEEFFF","AAABBBCCCDDDEEEFFF",
	};
	int expected[] = {0, 0, 0, 0, -32, -22, 9, 23};
	int i;

	for (i = 0; i < BK_LENGTH(inputs, char*); i += 2) {
		if (bk_strcmp(inputs[i], inputs[i + 1]) != expected[i / 2])
			return 1;
	}

	return 0;
}

int test_bk_memset(void *data, char* errmsg, int errdim) {
	char expected[] = "heellloooo";
	int const len = bk_strlen(expected) + 1;
	char input[len];

	bk_memset(input, 'h', 1);
	bk_memset(input + 1, 'e', 2);
	bk_memset(input + 3, 'l', 3);
	bk_memset(input + 6, 'o', 4);
	input[len - 1] = '\0';

	if (bk_strcmp(expected, input))
		return 1;

	return 0;
}


int test_bk_log(void *data, char* errmsg, int errdim) {
	unsigned powers[] = {
			1, 2, 3, 4, 5, 6, 7, 8,
			1, 9, 10, 50, 100, 500, 999, 1000, 1500
	};
	unsigned bases[] = {
			2, 2, 2, 2, 2, 2, 2, 2,
			10, 10, 10, 10, 10, 10, 10, 10, 10
	};
	int expected[] = {
			0, 1, 1, 2, 2, 2, 2, 3,
			0, 0, 1, 1, 2, 2, 2, 3, 3
	};
	int i;

	for (i = 0; i < BK_LENGTH(powers, unsigned); i++) {
		if (expected[i] != bk_log(bases[i], powers[i])) {
			errmsg[0] = '0' + i;
			errmsg[1] = '\0';
			return 1;
		}
	}

	return 0;
}

int test_bk_dev_line(void *data, char* errmsg, int errdim) {
	unsigned expected[] = {
			IL_DISK, IL_TAPE, IL_ETHERNET, IL_PRINTER, IL_TERMINAL,
	};
	unsigned lines[] = {
			IL_DISK, IL_TAPE, IL_ETHERNET, IL_PRINTER, IL_TERMINAL,
	};
	unsigned i;

	for (i = 0; i < BK_LENGTH(lines, unsigned); i++) {
		if (bk_dev_line((void *)DEV_REG_ADDR(lines[i], 7)) != expected[i])
			return 1;
	}

	return 0;
}
