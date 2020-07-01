#include "math.h"
#include "arch.h"
#include "string.h"
#include "utils.h"


long int bk_atoi (char const *src, unsigned base) {
	int res = 0;
	int coeff = 1;       
	int i = 0;
	
	if (base < 2) {
		return BK_E_INVARG;
	}
	
	while (src[i] != '\0') {
		if (src[i] < 48 || src[i] > 57) 
			return BK_E_INVARG;
			
		i += 1;
	}
	
	i = bk_strlen(src) - 1;
	
	while (src[i] != '\0') {
		res = res + coeff*(src[i] - '0');
		coeff *= base;
		i -= 1;
	}

	return res;
}

long int bk_atoi10 (char const *src) {
	return bk_atoi(src,10);
}

int bk_itoa (int n, char *dest, unsigned length) {
	int digits = bk_digits(n);

	/* If the number is negative, attempt to store a minus sign in front. */
	if (n < 0) {
		if (length > 1) {
			*dest = '-';
			dest++;
			length--;
		}

		n *= -1;
	}

	dest[bk_min(digits, length - 1)] = '\0';

	while (digits > 0) {
		if (digits < length)
			dest[digits - 1] = '0' + n % 10;

		n /= 10;
		digits--;
	}

	return BK_E_OK;
}

int bk_strlen (char const *str) {
 	int count;

	for (count = 0; *str != '\0'; str++, count++);

	return count;
}

int bk_strncpy(char *dest, char const *src, unsigned length) {
	int c;

	/* Copy as far as you can */
	for (c = 0; *src != '\0' && length > 0; dest++, src++, length--, c++)
		*dest = *src;

	*dest = '\0';

	return c;
}

int bk_strcpy(char *dest, char const *src) {
	int c;

	/* Copy as far as you can */
	for (c = 0; *src != '\0'; dest++, src++, c++)
		*dest = *src;

	return c;
}

int bk_strcmp(char *s, char *t) {
	int v;

	while (*s != '\0' && *t != '\0') {
		v = *s - *t;

		if (v)
			return v;

		s++;
		t++;
	}

	return *s - *t;
}


void* bk_memset(void *dest, char d, unsigned n) {
	int i;

	for (i = 0; i < n; i++)
		*(((char*) dest) + i) = d;

	return dest;
}

void* bk_memcpy(void *dest, void const *src, unsigned n) {
	void *res = dest;

	while (n > 0) {
		*((char*) dest) = *((char*) src);

		dest++;
		src++;
		n--;
	}

	return res;
}

void* memcpy(void *dest, void const *src, unsigned n) {
	return bk_memcpy(dest, src, n);
}
