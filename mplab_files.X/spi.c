/*
 *      2013 Elia Ritterbusch
 *      http://eliaselectronics.com
 *
 *      This work is licensed under the Creative Commons Attribution 3.0 Unported License.
 *      To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/
 */
#include <xc.h>
#include "spi.h"

void spi_init(void){
    // disable MSSP
    SSPCONbits.SSPEN = 0;
    // SDI set
    TRISC4 = 1;
    // SS set
    TRISA5 = 1;
    // SDO cleared
    // SCK cleared
    TRISC5 = 0;
    TRISC3 = 0;

    // SPI mode 0
    SSPCONbits.CKP = 0; // Idle state for clock is a low level
    SSPSTATbits.CKE = 1; // Transmit occurs on transition from active to Idle clock state
    SSPSTATbits.SMP = 1; // Input data sampled at end of data output time (took me 5 friggin' hours)
    SSPCONbits.SSPM = 0; // SPI Master mode, SCK = FOSC/4
    SSPCONbits.SSPEN = 1; // enable MSSP
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
	while( !PIR1bits.SSPIF ); // wait for buffer full
        PIR1bits.SSPIF = 0; // clear SSPIF
        *datain = SSPBUF; // read out data
        length--;
	dataout++;
        datain++;
    }
}

unsigned char spi_fast_shift(unsigned char data){
    SSPBUF = data;
    while( !PIR1bits.SSPIF ); // wait for buffer full
    PIR1bits.SSPIF = 0; // clear SSPIF
    return SSPBUF;
}