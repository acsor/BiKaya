#include "string.h"


// TODO Implement missing functions

int bka_strlen (char const *str) {
 	int cont = 0;

	while (*str != '\0') {
		cont++;
		str++;
	}

	return cont;
}
