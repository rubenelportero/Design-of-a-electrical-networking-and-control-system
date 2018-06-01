//Ruben Garcia Segovia - 15/05/2018.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *memoria;

int main (void) {
	while(1){
		char memo_disponible[2]; 
		system("du -bsh /home/pi/Desktop/basededatos/datos > memoria.txt"); //Comprobamos el espacio ocupado de los datos
		memoria=fopen("./memoria.txt","r");
		fgets(memo_disponible, 2, memoria);
		if(memo_disponible[0]=="6" && memo_disponible[1]=="," ){ //Si supera los 6 GB...
			system("rm -r /home/pi/Desktop/basededatos/datos"); //Borramos todo
		}
		fclose(memoria);
		sleep(300); //Dormimos 5 minutos
	}
}

