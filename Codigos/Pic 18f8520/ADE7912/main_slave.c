// Ruben Garcia Segovia
// ADC UNIDS-3 conectado por SPI junto con la Raspberry Pi 
// Simulando al conversor ADE7912.

// Librerias
#include "spi.h"
#include <delays.h>
#include <p18f8520.h>

//Configuracion del pic
#pragma config OSC= HS, WDT = OFF, LVP = OFF

//Los cuatro Bytes del ADC que se van a leer
char response1_1 = 0x00;
char response1_2 = 0x00; //Voltaje
char response1_3 = 0x00;
char response1_4 = 0x00;

char response2_1 = 0x00;
char response2_2 = 0x00; //Intensidad
char response2_3 = 0x00;
char response2_4 = 0x00;

char response3_1 = 0x00;
char response3_2 = 0x00; //Temperatura
char response3_3 = 0x00;
char response3_4 = 0x00;

//El dato recibido de la raspberry
char Raspi_spi = 0x00;
int contador = 0; //Contador = 0 para LN, 1 para L1, 2-L2, 3-L3
float sincro = 3840000; //Cada 2 minutos contabilizados...

//Son cuatro char, ya que guardamos las dos respuestas del conversor analógico digital
//Ya que tenemos dos datos a recuperar de 24 bits.
 
void main(void) {
	TRISDbits.TRISD0 = 1; //Aviso del ADE7912 Para saber cuando debemos pedir datos
	TRISDbits.TRISD7 = 0; //Para gobernar la raspberry pi
	TRISGbits.TRISG0 = 0; //Para gobernar el actuador
	TRISB=0XFF;
	TRISE=0X00;//gobernar todos los ADE7912
	LATE=0xFF;   //Para gobernar el ADC y la raspberry;
	LATGbits.LATG7=0; 

	//INICIO CONFIGURACION //

	LATDbits.LATD7=0; //Iniciamos RD7 a 0 para avisar a la raspberry pi.
	OpenSPI(SPI_FOSC_16,MODE_11,SMPEND); //Iniciamos SPI como maestro
	while(SSPBUF==1) {  //Comprueba que el ADC0 esté listo y lo configura
		LATEbits.LATE2=0; 
		WriteSPI(0x4C);
		ReadSPI();
		SSPBUF = SSPBUF&0x01;
		LATEbits.LATE2=1;
	}
	LATEbits.LATE2=0; 
	WriteSPI(0x40); //Registro de configuracion
	WriteSPI(0x81); //8Khz, BW seleccionado, modo multiples ADC...
	WriteSPI(0xE0); //Registro EMI
	WriteSPI(0xAA); 
	LATEbits.LATE2=1;

	while(SSPBUF==1) {  //Comprueba que el ADC1 esté listo y lo configura
		LATEbits.LATE3=0; 
		WriteSPI(0x4C);
		ReadSPI();
		SSPBUF = SSPBUF&0x01;
		LATEbits.LATE3=1;
	}
	LATEbits.LATE3=0; 
	WriteSPI(0x40); //Registro de configuracion
	WriteSPI(0x81); //8Khz, BW seleccionado, modo multiples ADC...
	WriteSPI(0xE0); //Registro EMI
	WriteSPI(0xAA);
	LATEbits.LATE3=1;

	while(SSPBUF==1) {  //Comprueba que el ADC2 esté listo y lo configura
		LATEbits.LATE4=0; 
		WriteSPI(0x4C);
		ReadSPI();
		SSPBUF = SSPBUF&0x01;
		LATEbits.LATE4=1;
	}
	LATEbits.LATE4=0; 
	WriteSPI(0x40); //Registro de configuracion
	WriteSPI(0x81); //8Khz, BW seleccionado, modo multiples ADC...
	WriteSPI(0xE0); //Registro EMI
	WriteSPI(0x55);
	LATEbits.LATE4=1;

	while(SSPBUF==1) {  //Comprueba que el ADC3 esté listo y lo configura
		LATEbits.LATE5=0; 
		WriteSPI(0x4C);
		ReadSPI();
		SSPBUF = SSPBUF&0x01;
		LATEbits.LATE5=1;
	}
	LATEbits.LATE5=0; 
	WriteSPI(0x40); //Registro de configuracion
	WriteSPI(0x81); //8Khz, BW seleccionado, modo multiples ADC...
	WriteSPI(0xE0); //Registro EMI
	WriteSPI(0x55);
	LATEbits.LATE5=1;

	//FIN CONFIGURACION //

	while(1){  //De forma indefinida...
	if(sincro>= 3840000) {
		LATEbits.LATE2=0;   //Llamada simultanea a todos los ADC 
		LATEbits.LATE3=0; 
		LATEbits.LATE4=0; 
		LATEbits.LATE5=0; 
		WriteSPI(0xB0); //Sync_snap
		WriteSPI(0x01); //Reset y sincronizacion
		LATEbits.LATE2=1;   //Llamada simultanea a todos los ADC 
		LATEbits.LATE3=1; 
		LATEbits.LATE4=1; 
		LATEbits.LATE5=1; 
		sincro = 0;
	}
	while(PORTDbits.PORTD0==1||contador>0); // Hasta que no nos avise el ADC de que está listo.
	switch(contador){
		case 0:
			LATEbits.LATE2=0;
			break;
		case 1:
			LATEbits.LATE3=0;
			break;
		case 2:
			LATEbits.LATE4=0;
			break;
		default:
			LATEbits.LATE5=0;
			break;
	}
	WriteSPI(0x04); //IWV
	ReadSPI();
	response1_1=SSPBUF;
	ReadSPI();
	response1_2=SSPBUF;
	ReadSPI();
	response1_3=SSPBUF;
	ReadSPI();
	response1_4=SSPBUF;
	WriteSPI(0x0C); //V1WV
	ReadSPI();
	response2_1=SSPBUF;
	ReadSPI();
	response2_2=SSPBUF;
	ReadSPI();
	response2_3=SSPBUF;
	ReadSPI();
	response2_4=SSPBUF;
	WriteSPI(0x14); //V2WV (Temperatura)
	ReadSPI();
	response3_1=SSPBUF;
	ReadSPI();
	response3_2=SSPBUF;
	ReadSPI();
	response3_3=SSPBUF;
	ReadSPI();
	response3_4=SSPBUF;
	switch(contador){
		case 0:
			LATEbits.LATE2=1;
			contador++;
			break;
		case 1:
			LATEbits.LATE3=1;
			contador++;
			break;
		case 2:
			LATEbits.LATE4=1;
			contador++;
			break;
		default:
			LATEbits.LATE5=1;
			contador = 0;
			break;
	}
		
/****************************************************************************/
	//                   VOLTAJE   //
	Nop();
	Nop();	 //Le dejamos un poco para que no sea inmediato
	Nop();
	OpenSPI(SLV_SSON,MODE_00,SMPEND); //Iniciamos SPI en modo ESCLAVO.
	SSPBUF = response1_1; //Cargamos el primer byte
	Nop();
	while(!SSPSTATbits.BF){  //Esperamos a que lo recojan
		LATD = 0XFF;  //Activamos el PIN RASPIENABLE
	}
	LATD = 0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response1_2; //Cargamos el segundo byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response1_3; //Cargamos el tercer byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response1_4;	//Cargamos el cuarto byte
 	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	//            INTENSIDAD          //
	SSPBUF = response2_1; //Cargamos el primer byte
	Nop();
	while(!SSPSTATbits.BF){  //Esperamos a que lo recojan
		LATD = 0XFF;  //Activamos el PIN RASPIENABLE
	}
	LATD = 0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response2_2; //Cargamos el segundo byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response2_3; //Cargamos el tercer byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response2_4;	//Cargamos el cuarto byte
 	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido´

	//           TEMPERATURA          //
	SSPBUF = response3_1; //Cargamos el primer byte
	Nop();
	while(!SSPSTATbits.BF){  //Esperamos a que lo recojan
		LATD = 0XFF;  //Activamos el PIN RASPIENABLE
	}
	LATD = 0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response3_2; //Cargamos el segundo byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response3_3; //Cargamos el tercer byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response3_4;	//Cargamos el cuarto byte
 	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	// 				CONFIRMACION			 //
	SSPBUF = 0xAA;	//Cargamos el byte de confirmacion
 	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	if (Raspi_spi == 0x10){  //Activamos el actuador si asi nos lo han pedido
		LATGbits.LATG0=1;
	} 
	if (Raspi_spi == 0x20){  //Desactivamos el actuador si asi nos lo han pedido
		LATGbits.LATG0=0;
	} 
	//Terminamos por ahora
	CloseSPI();
	}
}