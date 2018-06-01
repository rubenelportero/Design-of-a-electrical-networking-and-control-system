//Ruben Garcia Segovia 15/05/2018
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

//Funcion que hace la lectura del voltaje
void LecturaVoltaje(int linea, float *outVrms, float *outFrecuencia, float *outpico, int *detector_V, char* nombrelinea){
	char caracteres[40];
 	char *caracteres2;
	char *search = "	";
	float i = 0;
	double media = 0;
	double analizador=0;
	double analizador_0=0;
	float pico = 0;
	float rms=0;
	int estado = 0;
	int estado2=1;
	int cuenta = 0;
	float rms2=0;
	float Vmin= 0;
	int estado3 = 0;
	int estado4=1;
	int estadof = 0;
	int cuenta2 = 0;
	float frecuencia = 0;
	int festado = 0;
	float ftiempo = 0;
	float ftiempo2 = 0;
	float ftiempo3 = 0;
	float fcuenta = 0;
	float tiempo_actual;
	float tiempo_recogida;
	int detector = 0;
	float detector_inicio;
	float detector_final;
	float detector_tiempo;
	float detector_valor;
	float tiempo_corte;
	FILE *V;
	
	switch(linea) { //Seleccionamos que linea vamos a procesar
   case 1  :
	  V = fopen("./Voltaje/VL1.txt","r");
      break; 
	
   case 2  :
	  V = fopen("./Voltaje/VL2.txt","r");
      break; 
	  
   default :
	 V = fopen("./Voltaje/VL3.txt","r");
	}
	printf("\nAnalizando la traza...\n\n");
 	fgets(caracteres,40,V); //Cogemos la primera linea y sus 40 caracteres (que se ignoran)
	while(feof(V) == 0) { //Mientras que el archivo no este leido completamente...
		i++;
		fgets(caracteres,40,V); //Estraemos la linea correspondiente
		caracteres2 = strtok(caracteres, search);
		caracteres2 = strtok(NULL, search); //Hacemos una separacion entre el tiempo y el voltaje 
		analizador = atof(caracteres2); //Analizador contiene el valor en V.
// ***************** VRMS *********************************** //
		rms += (analizador)*(analizador); 
		cuenta++;
// ***************** VRMS *********************************** //
// ***************** Frecuencia *********************************** //
		if(atof(caracteres) > 1 && pico>200) { //'atof(caracteres)' es el tiempo
			if (analizador > (pico - 10) && festado == 0) {
				ftiempo = atof(caracteres);
				festado = 1;
			}
			if (analizador > (pico - 10) && festado == 2){
				ftiempo2 = atof(caracteres);
				frecuencia += 1/(ftiempo2-ftiempo);
				fcuenta++;
				festado = 3;
			}
			if (analizador < (pico/2) && festado == 1){
				festado = 2;
			}
			if (analizador < (pico/2) && festado == 3){
				festado = 0;
			}
		}
// ***************** Frecuencia *********************************** //
// ***************** Vmax *********************************** //
		if (((analizador) > pico) && analizador_0 > (analizador - 5) && analizador_0 != analizador){
			pico = analizador;
		}
		if (analizador < Vmin) {
			Vmin = analizador;
		}
// ***************** Vmax *********************************** //
//************************************************************//
	analizador_0 = analizador;
	}
	rms = sqrt(rms/cuenta); //Calculo final de Vrms
	frecuencia = frecuencia / fcuenta; //Calculo final de la frecuencia
	fclose(V);
	
	*outVrms = rms; //Sacamos el valor
	*outFrecuencia = 0.001 + frecuencia; //A prueba de errores. La frecuencia no puede ser 0.
	*outpico = pico; //Sacamos el valor.
	
}
//Funcion que hace la lectura de la intensidad. Es el mismo procesado que el voltaje, pero en la linea de intensidad.
void LecturaIntensidad(int linea, float *outIrms, float *outFrecuencia, float *outpico,int *detector_I, char* nombrelinea){
	char caracteres[40];
 	char *caracteres2;
	char *search = "	";
	double media = 0;
	double analizador=0;
	double analizador_0=0;
	double analizador_0_0=0;
	double pico = 0;
	float rms=0;
	int estado = 0;
	int estado2=1;
	int cuenta = 0;
	float rms2=0;
	float cuentapico=0;
	int estado3 = 0;
	int estado4=1;
	int cuenta2 = 0;
	int frecuencia_estado= 0;
	float frecuencia = 0;
	int festado = 0;
	float Imin = 0;
	float ftiempo = 0;
	float ftiempo2 = 0;
	float ftiempo3 = 0;
	float fcuenta = 0;
	float detector = 0;
	float detector_inicio;
	float detector_final;
	float detector_tiempo;
	float detector_valor;
	FILE *I;
	
	switch(linea) {
   case 1  :
	  I = fopen("./Intensidad/IL1.txt","r");
      break; 
	
   case 2  :
	  I = fopen("./Intensidad/IL2.txt","r");
      break; 
	  
   default :
	 I = fopen("./Intensidad/IL3.txt","r");
	}
	printf("\nAnalizando la traza...\n\n");
 	fgets(caracteres,40,I);
	while(feof(I) == 0) {
		fgets(caracteres,40,I);
		caracteres2 = strtok(caracteres, search);
		caracteres2 = strtok(NULL, search);
		analizador = atof(caracteres2);
// ***************** IRMS *********************************** //
			rms += (analizador)*(analizador);
			cuenta++;
// ***************** IRMS *********************************** //
// ***************** Frecuencia *********************************** //
		if(atof(caracteres) > 1 && pico > 200) {
			if (analizador > (pico - 10) && festado == 0) {
				ftiempo = atof(caracteres);
				festado = 1;
			}
			if (analizador > (pico - 10) && festado == 2){
				ftiempo2 = atof(caracteres);
				frecuencia += 1/(ftiempo2-ftiempo);
				fcuenta++;
				festado = 3;
			}
			if (analizador < (pico/2) && festado == 1){
				festado = 2;
			}
			if (analizador < (pico/2) && festado == 3){
				festado = 0;
			}
		}
// ***************** Frecuencia *********************************** //
// ***************** Imax *********************************** //
		if (((analizador) > pico) && analizador_0 > (analizador - 5) && analizador_0 != analizador){
			pico = analizador;
		}
		if (analizador < Imin) {
			Imin = analizador;
		}
// ***************** Imax *********************************** //
//************************************************************//
		analizador_0 = analizador;
	}
	rms = sqrt(rms/cuenta);
	frecuencia = frecuencia/fcuenta;
	fclose(I);
	*outIrms = rms;
	*outFrecuencia = 0.001 + frecuencia;
	*outpico = pico;
}
//Funcion que calcula las potencias de las fases dadas
void Potencias(int linea,float *activa,float *reactiva,float *aparente,float *factordepotencia, float Irms, float Vrms){
	FILE *V;
	FILE *I;
	char caracteresV[40];
	char caracteresI[40];
	char *caracteresV2;
	char *caracteresI2;
	char *search = "	";
	float potenciaactiva=0;
	float potenciareactiva=0;
	float potenciaaparente=0;
	float cosphi = 0;
	float cuenta=0;
	float Voltaje, Intensidad;
	switch(linea) { //Dependiendo de la linea que escojamos...
   case 1  :
	  V = fopen("./Voltaje/VL1.txt","r");
	  I = fopen("./Intensidad/IL1.txt","r");
      break; 
	
   case 2  :
	  V = fopen("./Voltaje/VL2.txt","r");
	  I = fopen("./Intensidad/IL2.txt","r");
      break; 
	  
   default :
	 V = fopen("./Voltaje/VL3.txt","r");
	 I = fopen("./Intensidad/IL3.txt","r");
	}
	printf("\nAnalizando la traza...\n\n");
 	fgets(caracteresV,40,V); //Siempre se descartan la primera linea
	fgets(caracteresI,40,I);
	while(feof(V) == 0 && feof(I) == 0) { //Mientras que el archivo no se termine...
		cuenta++;
		fgets(caracteresV,40,V);
		fgets(caracteresI,40,I);
		caracteresV2 = strtok(caracteresV, search);
		caracteresV2 = strtok(NULL, search);
		caracteresI2 = strtok(caracteresI, search);
		caracteresI2 = strtok(NULL, search);
		Voltaje = atof(caracteresV2); //Valor numerico del voltaje
		Intensidad = atof(caracteresI2); //Valor numerico de la intensidad
		potenciaactiva += Voltaje*Intensidad; 
	}
	potenciaactiva = potenciaactiva / cuenta;  //La potencia activa es la multiplicacion de valores instantaneos entre numero de valores cogidos
	potenciaaparente = Irms*Vrms; //La potencia aparente es la multiplicacion de sus valores medios
	potenciareactiva = potenciaaparente*potenciaaparente - potenciaactiva*potenciaactiva; //La reactiva es aparente menos activa debido a el triangulo dado
	cosphi = potenciaactiva/potenciaaparente; //El coseno es la potencia activa entre la aparente.
	*reactiva = sqrt(potenciareactiva); //Sacamos valores por la funcion
	*activa = potenciaactiva;
	*aparente = potenciaaparente;
	*factordepotencia = cosphi;
}
//Funcion que calcula el desfase entre dos fases dadas
float desfaseV(int linea1, int linea2, float frecuencia, float pico1,float pico2){
	FILE *V1;
	FILE *V2;
	char caracteresV1[40];
	char caracteresV2[40];
	char *caracteresV12;
	char *caracteresV22;
	char *search = "	";
	float Voltaje1, Voltaje2,Voltaje1_0=-1,Voltaje2_0=-1;
	float ftiempo1,ftiempo2;
	int fase1 = 1;
	float pivote=0;
	float resultado;
	switch(linea1) { //Elegimos lineas
   case 1  :
	  V1 = fopen("./Voltaje/VL1.txt","r");
      break; 
	
   case 2  :
	  V1 = fopen("./Voltaje/VL2.txt","r");
      break; 
	  
   default :
	 V1 = fopen("./Voltaje/VL3.txt","r");
	}
	switch(linea2) { //Elegimos lineas
   case 1  : 
	  V2 = fopen("./Voltaje/VL1.txt","r");
      break; 
	
   case 2  :
	  V2 = fopen("./Voltaje/VL2.txt","r");
      break; 
	  
   default :
	 V2 = fopen("./Voltaje/VL3.txt","r");
	}
	printf("\nAnalizando la traza...\n\n");
	fgets(caracteresV1,40,V1); //Ignoramos la primera linea
	fgets(caracteresV2,40,V2);
	while(feof(V1) == 0 && feof(V2) == 0) { //Mientras el archivo no se termine...
		fgets(caracteresV1,40,V1);
		fgets(caracteresV2,40,V2);
		caracteresV12 = strtok(caracteresV1, search);
		caracteresV12 = strtok(NULL, search);
		caracteresV22 = strtok(caracteresV2, search);
		caracteresV22 = strtok(NULL, search);
		Voltaje1 = atof(caracteresV12); //Voltaje uno
		Voltaje2 = atof(caracteresV22); //Voltaje dos
		if(Voltaje1_0 >= (pico1/2) && Voltaje1 < (pico1/2) ){ 
			ftiempo1 = atof(caracteresV1);
			fase1 = 0;
		}
		//Basicamente sacamos el tiempo entre que una fase cruza el valor pico/2 y lo hace la otra.
		if(Voltaje2_0>= (pico2/2) && Voltaje2 < (pico2/2) && fase1 == 0){
			ftiempo2 = atof(caracteresV2);
			fase1 = 3;
			}
			
		if(fase1==3){
			ftiempo1 = ftiempo1 - ftiempo2;
			pivote = 1/frecuencia;
			resultado = 360 * ftiempo1;
			resultado = resultado / pivote;
			if(resultado<0) resultado = 360 + resultado; //No sacamos valores negativos
			return resultado; //Devolvemos el resultado final
		}
		Voltaje1_0 = Voltaje1;
		Voltaje2_0 = Voltaje2;
		}
	}
//Funcion que guarda el dato procesado para su representacion grafica
void archivo(int anyo, int mes, int dia, int minuto, float dato, char* nombre){
	char string[40];
	FILE *TXT;
	snprintf(string,40,"../basededatos/%d/%d/%d/%s.txt",anyo,mes,dia,nombre);
	TXT=fopen(string,"a");
	fprintf(TXT,"%d %f\n",minuto, dato);
	fclose(TXT);
}
//Funcion que guarda todos los datos RAW del minuto
void guardararchivo(int anyo, int mes, int dia, int minuto, char* nombre,char* tipo){
		char string[40], stringsavedata[70];
		mkdir("../basededatos/datos",0777);
		snprintf(string,40,"../basededatos/datos/%d",anyo);
		mkdir(string,0777);
		snprintf(string,40,"../basededatos/datos/%d/%d",anyo,mes);
		mkdir(string,0777);
		snprintf(string,40,"../basededatos/datos/%d/%d/%d",anyo,mes,dia);
		mkdir(string,0777);
		snprintf(string,40,"../basededatos/datos/%d/%d/%d/%s",anyo,mes,dia,nombre);
		mkdir(string,0777);
		snprintf(stringsavedata,70,"mv -f ./%s/%s.txt ../basededatos/datos/%d/%d/%d/%s/%d.txt",tipo,nombre,anyo,mes,dia,nombre,minuto);
		printf("%s",stringsavedata);
		system(stringsavedata);
}

int main()
{
	FILE *TXT;
	float rms, frecuencia, pico=0, pico2=0,L1potenciactiva,L1potenciareactiva,L1potenciaparente,L1cosphi,L1Vrms,L1Irms,L1L2desfase;
	char string[40];
	int minutodeldia, dia, mes,anyo,detectorV=0, detectorI = 0;
	while(1) {  //Se ejecuta de forma infinita hasta que lo paremos manualmente
		if (fopen("../monitorizacion/IL3.txt","r") != NULL) { //El proceso empieza cuando el script 'spi' termina su parte
			//Primero calculamos la fecha en la que estamos
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			dia = tm.tm_mday;
			mes = tm.tm_mon + 1;
			anyo = tm.tm_year + 1900;
			minutodeldia = tm.tm_min + (tm.tm_hour*60);
			snprintf(string,30,"../basededatos/%d",anyo);
			mkdir(string,0777);
			snprintf(string,30,"../basededatos/%d/%d",anyo,mes);
			mkdir(string,0777);
			snprintf(string,30,"../basededatos/%d/%d/%d",anyo,mes,dia);
			mkdir(string,0777);
			//Creamos los directorios donde vamos a guardar la informacion
			//Movemos los archivos RAW a la carpeta correspondiente para empezar a tratarlos.
			system("mv -f ../monitorizacion/IL3.txt ./Intensidad/IL3.txt");
			system("mv -f ../monitorizacion/IL2.txt ./Intensidad/IL2.txt");
			system("mv -f ../monitorizacion/IL1.txt ./Intensidad/IL1.txt");
			
			system("mv -f ../monitorizacion/VL3.txt ./Voltaje/VL3.txt");
			system("mv -f ../monitorizacion/VL2.txt ./Voltaje/VL2.txt");
			system("mv -f ../monitorizacion/VL1.txt ./Voltaje/VL1.txt");
			system("mv -f ../monitorizacion/VLN.txt ./Voltaje/VLN.txt");
			
			//Se trata el voltaje
			LecturaVoltaje(1,&L1Vrms,&frecuencia,&pico,&detectorV, "VL1");	
			
			//Y se guardan los archivos de Vrms, frecuencia y su valor pico.
			archivo(anyo,mes,dia,minutodeldia,L1Vrms,"VRMS1");
			archivo(anyo,mes,dia,minutodeldia,frecuencia,"FrecV1");
			archivo(anyo,mes,dia,minutodeldia,pico,"PicoV1");
			
			//Lo mismo para la intensidad
			LecturaIntensidad(1,&L1Irms,&frecuencia,&pico2,&detectorI, "IL1");
			
			archivo(anyo,mes,dia,minutodeldia,L1Irms,"IRMS1");
			archivo(anyo,mes,dia,minutodeldia,frecuencia,"FrecI1");
			archivo(anyo,mes,dia,minutodeldia,pico2,"PicoI1");
			
			//Se calculan las potencias
			Potencias(1,&L1potenciactiva,&L1potenciareactiva,&L1potenciaparente,&L1cosphi,L1Irms,L1Vrms);
	
			//Se guardan el triangulo de potencias y el factor de potencia.
			archivo(anyo,mes,dia,minutodeldia,L1potenciactiva,"PotActL1");
			archivo(anyo,mes,dia,minutodeldia,L1potenciareactiva,"PotReactL1");
			archivo(anyo,mes,dia,minutodeldia,L1potenciaparente,"PotaparL1");
			archivo(anyo,mes,dia,minutodeldia,L1cosphi,"FdpL1");
			
			//Por ultimo, guardamos todos los datos RAW para su analisis mediante software especifico.
			guardararchivo(anyo,mes,dia,minutodeldia,"VL1","Voltaje");
			guardararchivo(anyo,mes,dia,minutodeldia,"VL2","Voltaje");
			guardararchivo(anyo,mes,dia,minutodeldia,"VL3","Voltaje");
			guardararchivo(anyo,mes,dia,minutodeldia,"VLN","Voltaje");
			
			guardararchivo(anyo,mes,dia,minutodeldia,"IL1","Intensidad");
			guardararchivo(anyo,mes,dia,minutodeldia,"IL2","Intensidad");
			guardararchivo(anyo,mes,dia,minutodeldia,"IL3","Intensidad");
			
		} else {
			sleep(1); //Dormimos 1seg para no dejar un bucle infinito.
		} 
	} 
	return 0;
}
