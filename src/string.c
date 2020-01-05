#include "string.h"
#include <stdbool.h>
#include "sys.h"


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



int bka_itoa(int n, char *dest, unsigned length){

	 int i = 0, dim =  0, n_tmp = n; 
	 bool tronc = false;
	 
	 if(n < 0) return BKA_E_GEN;
	 else if(n == 0){
	 	dest[i] = '0';
	 	dest[i+1] = '\0';
	 	return BKA_E_OK;
	 	}
	 else{
	 	while(n_tmp != 0){			//calcolo la dimensione del n° in input
	 		n_tmp = n_tmp/10;
	 		dim ++;
	 	}
	 }
	 
	 while (n != 0) 
		{ 
		    int r = n % 10; 
		    dest[i++] = r + '0'; 
		    n = n/10; 
		    
		    if(i > length){
		    	tronc = true;
		    }
		} 
		
	 int j = 0;
	 char copy[dim+1]; 		//array per invertire la stringa alla fine
	 copy[dim] = '\0';
	 i = dim-1;
	 
	 while(i >=0 && j < dim){
	 	copy[j] = dest[i];
	 	i--;
	 	j++;
	 }
	 
	 i = 0;
	 while(i < dim){
	 	dest[i] = copy[i];
	 	i++;
	 }
	 
	 dest[length] = '\0';       //così viene troncato il numero alla lunghezza richiesta
	 
	 if(tronc) return BKA_E_GEN;
	 else return BKA_E_OK;
}



int bka_strncpy(char *dest, char const *src, unsigned length){
	if(length == 0 || *src == '\0') return 0;
	int count = 0, i =0;		
	while(src[i] != '\0' && count < length){
	 	dest[i] = src[i];
	 	i++;
	 	count++;
	 }
	 dest[i] = '\0';
	 return count;
}



int bka_strcpy(char *dest, char const *src){
	if(*src == '\0') return 0;
	int count = 0, i =0;		
	while(src[i] != '\0'){
	 	dest[i] = src[i];
	 	i++;
	 	count++;
	 }
	 dest[i] = '\0';
	 return count+1;  //ritorna il n° di caratteri copiati incluso il '\0'
}
