#include "io.h"
#include "sys.h"
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

int test_bka_atoi(void *data, char* errmsg, int errdim);
int test_bka_itoa(void *data, char* errmsg, int errdim);
int test_bka_strlen(void *data, char* errmsg, int errdim);
int test_bka_strncpy(void *data, char* errmsg, int errdim);
int test_bka_strcpy(void *data, char* errmsg, int errdim);
int test_bka_strcmp(void *data, char* errmsg, int errdim);


int main (int argc, char *argv[]) {
	termreg_t * const term0 = (termreg_t*) DEV_REG_ADDR(IL_TERMINAL, 0);

	/* It appears that when a local-function array is too large to contain, the
	 * compiler implicitly invokes memcpy() to store it somewhere. Alas, in our
	 * cross-compiling setup that call isn't available, and prefixing the
	 * variable with the static storage modifier can do the trick.
	 */
	static test_t tests[] = {
		{"test_bka_atoi", test_bka_atoi},
		{"test_bka_itoa", test_bka_itoa},
		{"test_bka_strlen", test_bka_strlen},
		{"test_bka_strncpy", test_bka_strncpy},
		{"test_bka_strcpy", test_bka_strcpy},
		{"test_bka_strcmp", test_bka_strcmp},
	};
	test_function f;

	unsigned const buffsize = 128;
	char errmsg[buffsize];
	int const tests_no = BKA_LENGTH(tests, test_t);
	int i, failed_no = 0;

	for (i = 0; i < tests_no; i++) {
		errmsg[0] = '\0';
		bka_term_puts(term0, "Testing ", tests[i].name, "()... ", NULL);
		f = tests[i].f;

		/* If the test fails... */
		if (f(NULL, errmsg, buffsize)) {
			if (errmsg[0] == '\0')
				bka_strncpy(errmsg, "UNKNOWN", buffsize);

			bka_term_puts(term0, "FAILED.\n\tREASON: ", errmsg, "\n", NULL);
			failed_no++;
		} else {
			bka_term_puts(term0, "OK.\n", NULL);
		}
	}

	// printf(
	// 		"\nRUN %lu TESTS (%lu OK, %lu FAILED).\n",
	// 		tests_no, tests_no - failed_no, failed_no
	// );
	bka_term_puts(term0, "\nALL TESTS RUN.\n", NULL);

	return 0;
}


int test_bka_atoi(void *data, char *errmsg, int errdim) {
	char *input[] = {"0", "10", "123", "789"};
	int base[] = {10, 10, 10, 10};
	int expected[] = {0, 10, 123, 789};

	int const n = BKA_LENGTH(base, int);
	int i;

	for (i = 0; i < n; i++) {
		if (bka_atoi(input[i], base[i]) != expected[i])
			return 1;
	}

	return 0;
}

int test_bka_itoa(void *data, char *errmsg, int errdim) {
	unsigned const buffsize = 32;
	char buff[buffsize];

	int inputs[] = {0, 1, 10, 42, 129};
	char *expected[] = {"0", "1", "10", "42", "129"};
	unsigned const n = BKA_LENGTH(inputs, int);
	int i;

	for (i = 0; i < n; i++) {
		bka_itoa(inputs[i], buff, buffsize);

		if (bka_strcmp(expected[i], buff))
			return 1;
	}

	return 0;
}

int test_bka_strlen(void *data, char* errmsg, int errdim) {
	char *inputs[] = {"", "1", "one", "fortytwo"};
	unsigned const expected[] = {0, 1, 3, 8};
	int i;

	for (i = 0; i < BKA_LENGTH(inputs, char*); i++) {
		if (bka_strlen(inputs[i]) != expected[i])
			return 1;
	}

	return 0;
}

int test_bka_strncpy(void *data, char* errmsg, int errdim) {
	unsigned const buffsize = 64;
	char src[buffsize], dest[buffsize];
	char *inputs[] = {"", "1234567890", "fortytwo", "insanity"};
	int i;

	for (i = 0; i < BKA_LENGTH(inputs, char*); i++) {
		bka_strncpy(dest, src, buffsize);

		if (bka_strcmp(src, dest))
			return 1;
	}

	return 0;
}

int test_bka_strcpy(void *data, char* errmsg, int errdim) {
	unsigned const buffsize = 64;
	char src[buffsize], dest[buffsize];
	char *inputs[] = {"", "1234567890", "fortytwo", "insanity"};
	int i;

	for (i = 0; i < BKA_LENGTH(inputs, char*); i++) {
		bka_strcpy(dest, src);

		if (bka_strcmp(src, dest))
			return 1;
	}

	return 0;
}

int test_bka_strcmp(void *data, char* errmsg, int errdim) {
	static char *inputs[] = {
		// Equal strings
		"", "",
		"1234567890", "1234567890",
		"fortytwo", "fortytwo",
		"AAABBBCCCDDDEEEFFF","AAABBBCCCDDDEEEFFF",

		// Unequal strings
		"", " ",
		"12345 67890", "1234567890",
		"fortytwx", "fortytwo",
		"XAABBBCCCDDDEEEFFF","AAABBBCCCDDDEEEFFF",
	};
	static int expected[] = {0, 0, 0, 0, -32, -22, 9, 23};
	int i;

	for (i = 0; i < BKA_LENGTH(inputs, char*); i += 2) {
		if (bka_strcmp(inputs[i], inputs[i + 1]) != expected[i / 2])
			return 1;
	}

	return 0;
}
