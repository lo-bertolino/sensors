/* File:   main.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */

//#define TEST_SPI 1
#define TEST_DHT 1

#include <htc.h>
#include <pic16f876.h>
//#include "spi.h"
#include "dht.h"

#ifndef TEST_SPI
#    ifndef TEST_DHT
#        include "wl_module.h"
#        include "nRF24L01.h"
#    endif //DHT
#endif //SPI

__CONFIG (FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_ON);
#define _XTAL_FREQ 4000000


//inizializza uC
void Init(){
	//Port Configuration
	TRISA = 4;
	TRISB = 0;
	TRISC = 0;

	//spi_init(); //not using spi 4tm

#ifndef TEST_SPI
#    ifndef TEST_DHT
	wl_module_init();//initialise nRF24L01+ Module
    __delay_ms(50);	//wait for nRF24L01+ Module

    INTCONbits.PEIE = 1; // peripheral interrupts enabled
    INTCONbits.GIE = 1;  // global interrupt enable

    wl_module_tx_config( wl_module_TX_NR_0 ); //Config Module
#    endif //DHT
#endif //SPI
}

#if TEST_DHT

/*unsigned char SevenSeg (unsigned int value){
	if(value==0)
		value = 0b10111111;
	else if(value==1)
		value = 0b00000110;
	else if(value==2)
		value = 0b11011011;
	else if(value==3)
		value = 0b11001111;
	else if(value==4)
		value = 0b01100110;
	else if(value==5)
		value = 0b11101101;
	else if(value==6)
		value = 0b11111100;
	else if(value==7)
		value = 0b00000111;
	else if(value==8)
		value = 0b11111111;
	else if(value==9)
		value = 0b01100111;
	return value;
}*/

void main (){
	Init();
	unsigned char SevenSeg[] = {0b10111111, 0b00000110, 0b11011011, 0b11001111,
								0b01100110, 0b11101101, 0b11111100, 0b00000111,
								0b11111111, 0b01100111};
	int temp = 0, umid = 0;
	while(1){
		int res = dht_get(&temp, &umid);
		if(res==0){
			//temp++;
			//if(temp>=100)temp = 0;
			temp /= 10; //il risultato di dht_get e' 10 volte la temperatura reale (ha una cifra decimale)
			unsigned int unita = temp%10;
			unsigned int decine = (temp/10)%10;
		//on-device output
		PORTC = SevenSeg[unita];
			PORTB = decine?SevenSeg[decine]:0;
		}
		else{
			PORTB = SevenSeg[10]; //err
			PORTC = SevenSeg [res]; //0
		}
		__delay_ms(2300);
	}
}
#else
#    if TEST_SPI

void main (){
	Init();
	char *tmp, *waste;
	*waste = 0;
	while(1){
		wl_module_CSN_lo;
		spi_transfer_sync("Hello world ", waste, 12);
		wl_module_CSN_lo;
		__delay_ms(1000);
	}
}
#    endif

void main (){
	Init();
	char payload[7]; //Array for Payload

	while(1){
		int temp, umid;

		//nRF transmission

	}
}
#endif