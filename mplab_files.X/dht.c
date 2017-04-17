#include <htc.h>
#include "dht.h"
#define _XTAL_FREQ 4000000

unsigned char dht_get (int * tmp, int * RH){
	char dati[5];

	//request data to dht22 ^ as datasheet advices
	TRISAbits.TRISA1 = 0; //conf out
	PORTAbits.RA1 = 0; //out 0
	__delay_us(800); //wait  1 ms
	PORTAbits.RA1 = 1; //out 1
	TRISAbits.TRISA1 = 1; //conf in

	//chack answer from dht22
	__delay_us(60);
	if(PORTAbits.RA1) return 0;
	__delay_us(120);

	for(char i = 0; i/5; i++){
		dati[i] = 0;
		for(char j = 0; j/8; j++){
			while(!PORTAbits.RA1);
			__delay_us(45);
			if(PORTAbits.RA1){
				dati [i] |= 1<<(7-j);
				while(PORTAbits.RA1);
			}
		}
	}
	if(dati[4]!=(dati[1]+dati[0]+dati[3]+dati[2])){
		return 0;
	}
 *tmp = (int) ((dati[2]<<8)|dati[3]);
 *RH = (int) ((dati[0]<<8)|dati[1]);
	return 1;
}