//Ruben Garcia Segovia - 15/05/2018
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <time.h>


int main (void) {
	FILE *log;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t); 
	log=fopen("../basededatos/log.txt","a"); //Guardamos el inicio del servidor 
	fprintf(log,"\n%d-%d-%d %d:%d:%d - Inicio del servidor",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(log);
	while(1){ //Bucle infinito
	FILE *fp;  //Nos preparamos para abrir todos los archivos necesarios
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	FILE *fp5;
	FILE *fp6;
	FILE *fp7;
	FILE *actuador;
	fp=fopen("voltaje1temporal.txt","w");  //Los archivos RAW los guardamos siempre en archivos temporales
	fp2=fopen("voltaje2temporal.txt","w");
	fp3=fopen("voltaje3temporal.txt","w");
	fp4=fopen("voltajentemporal.txt","w");
	fp5=fopen("intensidad1temporal.txt","w");
	fp6=fopen("intensidad2temporal.txt","w");
	fp7=fopen("intensidad3temporal.txt","w");
	fprintf(fp, "Ruben Garcia Segovia - VoltajeL1");
	fprintf(fp2, "Ruben Garcia Segovia - VoltajeL2");
	fprintf(fp3, "Ruben Garcia Segovia - VoltajeL3");
	fprintf(fp4, "Ruben Garcia Segovia - VoltajeLN");
	fprintf(fp5, "Ruben Garcia Segovia - IntensidadL1");
	fprintf(fp6, "Ruben Garcia Segovia - IntensidadL2");
	fprintf(fp7, "Ruben Garcia Segovia - IntensidadL3");
	printf("Pruebas de SPI_Ruben_Garcia_Segovia\n");
	
	double BILLION= 1000000000; 
	int actuador_memory = 0;
	struct timespec time_origen, time_actual;
	double accum=0;
	float huecoV=0;
	float huecoI=0;
	int huecoVestado=0;
	int huecoIestado=0;
	char caractuador[10];
	wiringPiSetup () ;
	pinMode (1, INPUT); 
	
	if(wiringPiSPISetup(0, 1000000) < 0) //Iniciamos el SPI 500Khz, SPI0 como CS
	{
		fprintf(stderr, "Error al leer el SPI: %s",strerror (errno));
		exit(1);
	}
	unsigned char ByteSPI[7]; //Variable para transmitir y recibir datos por spi
	long voltaje,intensidad;
	float voltajefinal,intensidadfinal;
	double notacion,notacion2;
	delay(1000); //1 segundo para dar tiempo a que se establezca el chip SPI
	//Cargamos los datos
	clock_gettime(CLOCK_MONOTONIC, &time_origen); //El reloj monotonic da siempre un valor positivo
	//e incremental en el tiempo, se utiliza para saber el tiempo que ha tardado en ejecutar 'x' lineas de codigo.
	//Lo ejecutamos aqui para tomar el primer valor
while(accum<=60){ //accum son los segundos transcurridos desde el anterior clock_gettime. Este bucle se realiza una vez por segundo
	actuador=fopen("actuador.txt","r");
	fgets(caractuador, 10, actuador); //Leemos el estado del actuador
	fclose(actuador);
	while(digitalRead(1)==LOW); //Esperamos a que el pic nos de la orden de comunicar
	wiringPiSPIDataRW (0, ByteSPI, 1);  //Primer Byte
	ByteSPI[1] = ByteSPI[0];
	while(digitalRead(1)==LOW);
	wiringPiSPIDataRW (0, ByteSPI, 1);  //Segundo Byte
	ByteSPI[2] = ByteSPI[0];
	while(digitalRead(1)==LOW);
	wiringPiSPIDataRW (0, ByteSPI, 1);  //Tercer Byte
	ByteSPI[3] = ByteSPI[0];
	while(digitalRead(1)==LOW);
	wiringPiSPIDataRW (0, ByteSPI, 1);  //Cuarto Byte
	ByteSPI[4] = ByteSPI[0];
	
	if (caractuador[0] == '1'){ //Si el actuador esta activado...
		ByteSPI[0] = 0x10; //Se manda un 0x10 al pic (activar actuador)
		if(actuador_memory==0){ //Si no estaba activado de antes...
			FILE *log;
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			log=fopen("../basededatos/log.txt","a"); //Se apunta en el log
			fprintf(log,"\n%d-%d-%d %d:%d:%d - Encendido del actuador",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			fclose(log);
		}
		actuador_memory = 1;
	} else if(caractuador[0] == '0' && actuador_memory == 1){  //Si no esta activado y antes si lo estaba
		ByteSPI[0]= 0x20; //Se manda 0x20 al pic (Desactivar actuador)
		FILE *log;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		log=fopen("../basededatos/log.txt","a");//Se apunta en el log
		fprintf(log,"\n%d-%d-%d %d:%d:%d - Apagado del actuador",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		actuador_memory = 0;
	} else {
		ByteSPI[0]= 0x20; //Se manda el 0x20
	}
	while(digitalRead(1)==LOW); //Nos esperamos a que el pic nos de permite de comunicacion
	wiringPiSPIDataRW (0, ByteSPI, 1);  //Byte de confirmacion y envio de actuador
	
	//Calculo del voltaje
	voltaje = ByteSPI[1] * 256;
	voltaje = voltaje + ByteSPI[2];
	voltajefinal = (voltaje*4.096)/4096;
	notacion = (voltajefinal * 100);
	
	//Calculo de la intensidad
	intensidad = ByteSPI[3] * 256;
	intensidad = intensidad + ByteSPI[4];
	intensidadfinal = (intensidad*4.096)/4096;
	notacion2 = (intensidadfinal * 100);
	
	//Detectamos posibles cortes
	if(huecoV>3000 && huecoVestado==0){
		huecoVestado=1;
	}
	if(huecoI>3000 && huecoIestado==0){
		huecoIestado=1;
	} //Tres mil muestras es aproximadamente 1 segundo
	if(huecoVestado==1 && huecoV >3000){
		FILE *log;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		log=fopen("../basededatos/log.txt","a"); //Escribimos en el log el corte
		fprintf(log,"\n%d-%d-%d %d:%d:%d - Corte en el Voltaje",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		log=fopen("../monitorizacion/estado.txt","w");
		fprintf(log,"1");
		fclose(log);
		huecoVestado = 2;
	}
	if(huecoVestado==2 && huecoV ==0){
		FILE *log;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		log=fopen("../basededatos/log.txt","a"); //Escribimos en el log la reanudacion
		fprintf(log,"\n%d-%d-%d %d:%d:%d - Reanudado el Voltaje",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		log=fopen("../monitorizacion/estado.txt","w");
		fprintf(log,"0");
		fclose(log);
		huecoVestado = 0;
	}
		if(huecoIestado==1 && huecoI >3000){
		FILE *log;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		log=fopen("../basededatos/log.txt","a");
		fprintf(log,"\n%d-%d-%d %d:%d:%d - Corte en el Intensidad",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		log=fopen("../monitorizacion/estado.txt","w");
		fprintf(log,"1");
		fclose(log);
		huecoIestado = 2;
	}
	if(huecoIestado==2 && huecoI ==0){
		FILE *log;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		log=fopen("../basededatos/log.txt","a");
		fprintf(log,"\n%d-%d-%d %d:%d:%d - Reanudada la Intensidad",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fclose(log);
		log=fopen("../monitorizacion/estado.txt","w");
		fprintf(log,"0");
		fclose(log);
		huecoIestado = 0;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &time_actual); //Tomamos el segundo valor del reloj monotonic
	//Con la siguiente ecuacion sacamos el tiempo transcurrido en notacion cientifica.
	accum = (time_actual.tv_sec - time_origen.tv_sec) + (time_actual.tv_nsec - time_origen.tv_nsec) / BILLION ;
	
	if(ByteSPI[0]==0xAA){  //Se comprueba byte de confirmacion
		printf("%f\n",accum);
		fprintf(fp,"\n%le	%le",accum,notacion);
		fprintf(fp2,"\n%le	%le",accum,notacion);
		fprintf(fp3,"\n%le	%le",accum,notacion);
		fprintf(fp4,"\n%le	0",accum);
		fprintf(fp5,"\n%le	%le",accum,notacion2);
		fprintf(fp6,"\n%le	%le",accum,notacion2);
		fprintf(fp7,"\n%le	%le",accum,notacion2);
		if(notacion<50){
			huecoV++;
		} else {
			huecoV=0;
		}
		if(notacion2<50){
			huecoI++;
		} else {
			huecoI=0;
		}
	}
}
	//Cerramos ficheros
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	fclose(fp7);
	rename("voltaje1temporal.txt","VL1.txt");
	rename("voltaje2temporal.txt","VL2.txt");
	rename("voltaje3temporal.txt","VL3.txt");
	rename("voltajentemporal.txt","VLN.txt");
	rename("intensidad1temporal.txt","IL1.txt");
	rename("intensidad2temporal.txt","IL2.txt");
	rename("intensidad3temporal.txt","IL3.txt");//En cuanto se renombra este fichero, el script lectura.c se ejecuta
	}
	return (0) ;
}
