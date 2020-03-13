/*Gestire interrupts:
	1)identificare sorgente dell'interrupt
		-linea:registro cause.IP
		-device sulla linea(>3):interrupting device bit map
	2)Acknowledgment dell'interrupt
		-scrivere un comando di ack o un nuovo comando nel registro 		 device
		
Conviene utilizzare l'interval timer perchè il PLT non è presente su uARM
*/
