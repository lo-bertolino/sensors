
#include <htc.h>
#include "dht.h"
#define _XTAL_FREQ 4000000

unsigned char dht_get (int * tmp, int * RH){
	char dati[5], i, j;

	//request data to dht22 ^ as datasheet advices
	TRISAbits.TRISA1 = 0; //conf out
	PORTAbits.RA1 = 1; //out 0
	__delay_ms(1);
	PORTAbits.RA1 = 0; //out 0
	__delay_ms(1);
	PORTAbits.RA1 = 1; //out 1
	__delay_us(30);
	TRISAbits.TRISA1 = 1; //conf in

	//check answer from dht22
	__delay_us(40); //metà di 80, Trel
	if(PORTAbits.RA1) return 2; //no response error 1
	__delay_us(80); //aspetta Treh
	if(PORTAbits.RA1 = 0) return 3; //no response error 2
	__delay_us(40);

	for(i = 0; i<5; i++){//per i 5 byte
		dati[i] = 0; //reset byte
		for(j = 0; j<8; j++){//per gli 8 bit
			while(!PORTAbits.RA1); //aspetta rising edge
			__delay_us(10); //aspetta
			if(PORTAbits.RA1) return 4; //alto durante un periodo basso
			__delay_us(20);
			if(PORTAbits.RA1){//se alto
				__delay_us(30); //aspetta altri 30 usec
				if(!PORTAbits.RA1)return 4; //se basso, sync error
				dati[i] |= 1<<(7-j); //bit a 1
				while(PORTAbits.RA1); //aspeta falling edge
			}
			else{
				dati[i] &= 0<<(7-j); //bit a 0
			}
		}
	}


	if(dati[4]!=(dati[1]+dati[0]+dati[3]+dati[2])){
		return 1; //data error
	}
	*tmp = (int) ((dati[2]<<8)|dati[3]);
	*RH = (int) ((dati[0]<<8)|dati[1]);
	return 0;
}