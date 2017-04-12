/* File:   main.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */

#include <htc.h>
#include <pic16f876.h>
#include "spi.h"
#include "nRF24L01.h"
#include "wl_module.h"

__CONFIG (FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_ON & CPD_OFF & WRT_ON);
#define _XTAL_FREQ 4000000

#define TEST_SPI
//inizializza uC
void Init(){
	//Port Configuration
	TRISA = 0;
	TRISB = 0;
	TRISC = 0;

	spi_init();

	wl_module_init();//initialise nRF24L01+ Module
    __delay_ms(50);	//wait for nRF24L01+ Module

    INTCONbits.PEIE = 1; // peripheral interrupts enabled
    INTCONbits.GIE = 1;  // global interrupt enable

    wl_module_tx_config( wl_module_TX_NR_0 ); //Config Module
}

bit dht_get(int * temp, int * RH ){
    char dati[5];

    //request data to dht22 ^ as datasheet advices
	TRISCbits.TRISC1 = 0; //conf out
	PORTCbits.RC1 = 0; //out 0
	__delay_ms(1); //wait  1 ms
	PORTCbits.RC1 = 1; //out 1
	__delay_us(30); //wait 30 us
	TRISCbits.TRISC1 = 1; //conf in

    //chack answer from dht22
    __delay_us(40);
	if(PORTCbits.RC1) return 0;
    __delay_us(120);

	for(char i = 0;i/5;i++){
		dati[i] = 0;
		for(char j = 0;j/8;j++){
            while (!PORTCbits.RC1);
            __delay_us(30);
            if(PORTCbits.RC1){
				dati [i] |= 1<<(7-j);
				while(PORTCbits.RC1);
            }
        }
	}
	if(dati[4]!=(dati[1]+dati[0]+dati[3]+dati[2] & 0xFF)){
        return 0;
    }
    &temp = (dati[2]<<8)|dati[3];
	&RH = (dati[0]<<8)|dati[1];
    return 1;
}


#ifndef TEST_SPI

void main (){
	Init();
	char payload[7]; //Array for Payload

    while(1){
		int temp, umid;

        //nRF transmission

    }
}
#else

void main (){
	Init();
	char tmp = 'a';
	while(1){
		spi_transmit_sync(tmp, 1);
		__delay_ms(1000);
	}
}
#endif
