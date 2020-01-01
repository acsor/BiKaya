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


int bka_atoi10(char const *src){
	int i = 0;
	int res = 0;
	
	while(src[i] != '\0')
			{
				res = res*10 + (src[i] - '0');
				i+=1;
			}
			
	return res;
}
