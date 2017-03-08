/* File:   newmain.c
 * Author: Lorenzo Bertolino
 *
 * Created on 20 december 2016, 9:31
 */

#include<xc.h>
/*
#include<stdio.h>
#include<stdlib.h>
#define BtBp "%c%c%c%c%c%c%c%c"
#define BtB(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
*/
__CONFIG (FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_ON & CPD_OFF & WRT_ON);
#define _XTAL_FREQ 4000000


//inizializza uC
void Init(){
	/* per prova:
	return;
	//*/
	
	//* impostazioni reali
	//Port Configuration
	TRISA=0b11111111;
	TRISB=0;
	//Convertitore on, Fosc/32
	ADCON0=0b10000001;
	//allineamento a sinistra e solo RA0 analogico, con Vref interna
	ADCON1=0b01001110;//*/
}

//leggi da AN0
unsigned int AN0Read(){
	/* per prova
	return 256;
	//*/
	
	//* impostazioni reali
	__delay_ms(1);
	GO_DONE=1;//convertitore on su canale 0
	__delay_ms(1);
	return ADRESH;//*/
}
unsigned int decVal;
int main() {
	Init();
	while(1){
		decVal=(AN0Read()*80)/256;//richiederà un po'di cicli
		PORTB=(decVal%10)<<4|((decVal/10)%10);//anche questo
	//printf(""BtBp"\n",BtB(portb));	//debug
	}
    return 0;
}
