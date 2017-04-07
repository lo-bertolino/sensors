/* File:   newmain.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */

#include <htc.h>
#include "spi.h"
#include "nRF24L01.h"
#include "wl_module.h"

__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_ON & CPD_OFF & WRT_ON);
#define _XTAL_FREQ 4000000

//inizializza uC
void Init(){
	//Port Configuration
	TRISA=0b11111111;
	TRISB=0;
    TRISC=0;
    OPTION_REG|=0b10000000;
	//Convertitore on, Fosc/32
	ADCON0=0b10000001;
	//allineamento a sinistra e solo RA0 analogico, con Vref interna
	ADCON1=0b00001110;//*/
    wl_module_init();	//initialise nRF24L01+ Module
    _delay_10ms(5);	//wait for nRF24L01+ Module
    
    INTCONbits.PEIE = 1; // peripheral interrupts enabled
    INTCONbits.GIE = 1;  // global interrupt enable
    
    wl_module_tx_config( wl_module_TX_NR_0 ); //Config Module
    spi_init();
}

//leggi da AN0
unsigned int AN0Read(){
	/* per prova
	return 256;//*/
	//* impostazioni reali
	__delay_ms(1);
	GO_DONE=1;//convertitore on su canale 0
	while(GO_DONE);
	return ADRESH;//*/
}

unsigned int SevenSeg(unsigned int value){
    switch(value){
        case 0:
            return 0b00111111;
        case 1:
            return 0b00000110;
        case 2:
            return 0b01011011;
        case 3:
            return 0b01001111;
        case 4:
            return 0b01100110;
        case 5:
            return 0b01101101;
        case 6:
            return 0b01111100;
        case 7:
            return 0b00000111;
        case 8:
            return 0b01111111;
        case 9:
            return 0b01100111;
    }
}


int main() {
    unsigned char payload[wl_module_PAYLOAD]; //Array for Payload
    unsigned char maincounter =0;
    unsigned char k;
    
    Init();
    
    while(1){
        unsigned int decVal=(AN0Read()*80)/256;
        unsigned int unita=decVal%10;
        unsigned int decine=(decVal/10)%10;
        
        //on-device output
        PORTC=SevenSeg (unita);
        if(decine)
            PORTB=SevenSeg (decine);
        else
            PORTB=0;
        //nRF transmission
        
        
        
        __delay_ms(500);
    }
}
