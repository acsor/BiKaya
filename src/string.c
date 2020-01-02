#include "string.h"
#include "sys.h"

// TODO Implement missing functions

int bka_strlen (char const *str) {
 	int cont = 0;
 	
	while (*str != '\0') {
		cont++;
		str++;
	}

	return cont;
}


int bka_atoi(char const *src, unsigned base){
	int res = 0;
	int coeff = 1;       
	int i = 0;
	
	if(base < 2)
	{
		return BKA_E_INVARG; 
	}
	
	while (src[i] != '\0')
	{
		if(src[i] < 48 || src[i] > 57) 
			return BKA_E_INVARG;
			
		i+=1;
	}
	
	i = bka_strlen(src) - 1;
	
	while(src[i] != '\0')
	{
		res = res + coeff*(src[i] - '0');
		coeff *= base;
		i-=1;
	}
	return res;
}


long int bka_atoi10 (char const *src) {
	
	return bka_atoi(src,10);
}
