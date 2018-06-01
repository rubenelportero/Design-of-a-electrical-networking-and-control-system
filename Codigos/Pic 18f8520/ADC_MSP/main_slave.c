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
char response = 0x00;
char response2 = 0x00;
char response3 = 0x00;
char response4 = 0x00;
//El dato recibido de la raspberry
char Raspi_spi = 0x00;

//Son cuatro char, ya que guardamos las dos respuestas del conversor analógico digital
//Ya que tenemos dos datos a recuperar de 12 bits.

unsigned char x = 0x06; //Primera palabra de configuracion
char x2= 0x00; //Segunda palabra de configuracion CH0
char x3= 0x64; //Segunda palabra de configuracion CH1
 
void main(void) {
TRISDbits.TRISD0 = 0; //Para gobernar la transmision con la Raspberry
TRISGbits.TRISG0 = 0; //Para gobernar el actuador
TRISB=0XFF;
TRISE=0X00;
LATE=0x00;   //Para gobernar el ADC y la raspberry;
LATGbits.LATG0=0; 

while(1){  //De forma indefinida...
	LATD = 0x00;  //Iniciamos a 0 el puerto D
	OpenSPI(SPI_FOSC_16,MODE_11,SMPEND); //Iniciamos SPI como maestro
	/*                  CH0                  */
	LATEbits.LATE3=0; 
	//Con el RE3 habilitamos el chip select del ADC
	WriteSPI(x); //Escribimos palabra de configuracion 1
	WriteSPI(x2); //Escribimos palabra de configuracion 2
	response = 0x0f& SSPBUF;
	//Leemos la respuesta del buffer y hacemos la mascara 
	//Para borrar los 4 primeros bits que no nos interesan
	response2 = ReadSPI();
	//Ahora leemos con ReadSPI porque solo nos interesa leer el siguiente
	//dato e ignoramos lo que envie el protocolo SPI
    LATEbits.LATE3 = 1;
	Nop();
	Nop();		//Le damos un poco de tiempo para crear un pulso en alto a RE3
	Nop();
	/*                  CH1                  */
	LATEbits.LATE3 = 0; 
	//Con el RE3 habilitamos el chip select del ADC
	WriteSPI(x); //Escribimos palabra de configuracion 1
	WriteSPI(x3); //Escribimos palabra de configuracion 2

	response3 =0x0f & SSPBUF;
	//Leemos la respuesta del buffer y hacemos la mascara 
	//Para borrar los 4 primeros bits que no nos interesan
	response4 = ReadSPI();
	//Ahora leemos con ReadSPI porque solo nos interesa leer el siguiente
	//dato e ignoramos lo que envie el protocolo SPI
    LATEbits.LATE3=1;
	CloseSPI(); //Cerramos SPI
	
/****************************************************************************/
	Nop();
	Nop();	 //Le dejamos un poco para que no sea inmediato
	Nop();
	OpenSPI(SLV_SSON,MODE_00,SMPEND); //Iniciamos SPI en modo ESCLAVO.
	SSPBUF = response; //Cargamos el primer byte
	Nop();
	while(!SSPSTATbits.BF){  //Esperamos a que lo recojan
		LATD = 0XFF;  //Activamos el PIN RASPIENABLE
	}
	LATD = 0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response2; //Cargamos el segundo byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response3; //Cargamos el tercer byte
	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

	SSPBUF = response4;	//Cargamos el cuarto byte
 	Nop();
	while(!SSPSTATbits.BF){ //Esperamos a que lo recojan
		LATD=0XFF; //Activamos el PIN RASPIENABLE
	}
	LATD=0X00;
	Raspi_spi = SSPBUF; //Recuperamos el dato recibido

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