#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <architecture.h>

char* bka_archictecture();

int main (int argc, char *argv[]) {
	char *arch = bka_archictecture();

	printf("Selected architecture is %s\n", arch);

	free(arch);

	return EXIT_SUCCESS;
}

#if defined(UARM)
char* bka_archictecture() {
	return strdup("uARM");
}
#elif defined(UMIPS)
char* bka_archictecture() {
	return strdup("uMIPS");
}
#endif
