/*
 *      2013 Elia Ritterbusch
 *      http://eliaselectronics.com
 *
 *      This work is licensed under the Creative Commons Attribution 3.0 Unported License.
 *      To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/
 */
#include <htc.h>
#include "spi.h"
#include <pic18f4550.h>

void spi_init(void){
    // disable MSSP
	SSPCON1bits.SSPEN = 0;
    // SDI1 set
	TRISBbits.TRISB0 = 1; // configure RC4 as input
	//ANSELCbits.ANSC4 = 0; // enable digital input buffer on RC4 //chissàperchénonc'è
    // SS1 set
	TRISAbits.TRISA5 = 1;
    // SDO1 cleared
    // SCK1 cleared
	TRISCbits.TRISC7 = 0;
	TRISBbits.TRISB1 = 0;

    // SPI mode 0
	SSPCON1bits.CKP = 0; // Idle state for clock is a low level
	SSPSTATbits.CKE = 1; // Transmit occurs on transition from active to Idle clock state
	SSPSTATbits.SMP = 1; // Input data sampled at end of data output time (took me 5 friggin' hours)
	SSPCON1bits.SSPM = 0; // SPI Master mode, SCK = FOSC/4
	SSPCON1bits.SSPEN = 1; // enable MSSP
}

void spi_transmit_sync(unsigned char * data, unsigned int length){
    unsigned char tmp;
    while(length){
	SSPBUF = *data;
	while( !PIR1bits.SSPIF ); // wait for buffer full
		PIR1bits.SSPIF = 0; // clear SSPIF
		tmp = SSPBUF; // read out data
        length--;
	data++;
    }
}

void spi_transfer_sync(unsigned char * dataout, unsigned char * datain, unsigned int length){
    while(length){
		SSPBUF = *dataout;
		while(!PIR1bits.SSPIF); // wait for buffer full
		PIR1bits.SSPIF = 0; // clear SSPIF
		*datain = SSPBUF; // read out data
        length--;
	dataout++;
        datain++;
    }
}

unsigned char spi_fast_shift(unsigned char data){
	SSPBUF = data;
	while(!PIR1bits.SSPIF); // wait for buffer full
	PIR1bits.SSPIF = 0; // clear SSPIF
	return SSPBUF;
}