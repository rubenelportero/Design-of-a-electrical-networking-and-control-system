//Ruben Garcia Segovia - 15/05/2018
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *actuador; 

int main (void) {
	char ftp_past; //Variable para saber si se ha cambiado el estado del ftp
	while(1){
		char ftp_actuador[10]; //Almacenamos el documento de ftp.
		actuador=fopen("/home/pi/Desktop/monitorizacion/ftp.txt","r");
		fgets(ftp_actuador, 10, actuador);
		fclose(actuador);
		if(ftp_actuador[0]=='1'){ //Si es 1 esta activado
			if(ftp_past=='0'){ //Si ha cambiado escribimos, sino, no.
				FILE *log;
				time_t t = time(NULL);
				struct tm tm = *localtime(&t);
				log=fopen("../basededatos/log.txt","a"); //Escribimos en el log
				fprintf(log,"\n%d-%d-%d %d:%d:%d - Encendido del FTP",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				system("sudo /etc/init.d/proftpd start"); //Inicia el servidor FTP
			}
		} else {
			if(ftp_past=='1') //Si ha cambiado de estado escribimos, sino, no.
			{
				FILE *log;
				time_t t = time(NULL);
				struct tm tm = *localtime(&t);
				log=fopen("../basededatos/log.txt","a"); //Escribimos en el log
				fprintf(log,"\n%d-%d-%d %d:%d:%d - Apagado del FTP",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fclose(log);
				system("sudo /etc/init.d/proftpd stop"); //Para el servidor FTP
			}
		}
		sleep(1); //Dormimos un segundo, para no crear un bucle while(1) infinito
		ftp_past = ftp_actuador[0]; 
	}
}
