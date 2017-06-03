/* File:   newmain.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */

#pragma config PLLDIV = 4, CPUDIV = OSC1_PLL2, USBDIV = 1, FOSC = INTOSC_HS, FCMEN = OFF, IESO = OFF, PWRT = OFF
#pragma config BOR = ON, BORV = 3, VREGEN = OFF
#pragma config WDT = OFF, WDTPS = 32768
#pragma config CCP2MX = ON, PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON
#pragma config STVREN = ON, LVP = ON, ICPRT = OFF, XINST = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <htc.h>
#define _XTAL_FREQ 16000000

//inizializza uC
void Init(){
	//Port Configuration
	TRISA=0b11111111;
	TRISB=0;
    TRISC=0;
	//    OPTION_REG|=0b10000000;
	//Convertitore on, Fosc/32
	ADCON0=0b10000001;
	//allineamento a sinistra e solo RA0 analogico, con Vref interna
	ADCON1=0b00001110;//*/
}

//leggi da AN0
unsigned int AN0Read(){
	__delay_ms(1);
	GO_DONE=1;//convertitore on su canale 0
	while(GO_DONE);
	return ADRESH;//*/
}

/*unsigned int SevenSeg(unsigned int value){
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
 */

int main() {
    Init();
	/*    while(1){
			unsigned int decVal=(AN0Read()*80)/256;
			unsigned int unita=decVal%10;
			unsigned int decine=(decVal/10)%10;

			//        PORTC=SevenSeg (unita);

			if(decine);
				//        PORTB=SevenSeg (decine);
			else
				PORTB=0;
			__delay_ms(498);
		}*/
}
