/* File:   newmain.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */

#pragma config PLLDIV = 4, CPUDIV = OSC1_PLL2, USBDIV = 1, FOSC = HS, FCMEN = OFF, IESO = OFF, PWRT = OFF
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

#include "dht.h"
#define _XTAL_FREQ 16000000

unsigned char humid, res, time_out;
char temp;
//inizializza uC
void Init(){
	TRISA = 0xff;
	TRISB = 0;
}

int main(){
	Init();

    while(1){
		DHTHandler();
		if(res=='s') PORTB = temp;
		else if(res=='r') PORTB = 0xf1;
		else if(res=='t') PORTB = 0xf2;
		else if(res=='c') PORTB = 0xf3;

		__delay_ms(2000);
	}
}

/*unsigned int SevenSeg[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111, 0b01111001}; //l'ultimo è una E, per gli errori
 */

// Connection pin between PIC18F4550 and DHT22 sensor
#define DPin PORTCbits.RC0            // Pin mapped to PORTB.0
#define DPinDir TRISCbits.TRISC0      // Pin direction mapped to TRISB.0

char readDHT (){
	unsigned char i, k, dat = 0; // k is used to count 1 bit reading duration
	if(!time_out) return 0;
	for(i = 0; i<8; i++){
		k = 0;
		while(!DPin){ // Wait until pin goes high
			k++;
			if(k>100){
				time_out = 0;
				return 0;
			}
			__delay_us(1);
		}
		__delay_us(30);
		if(!DPin) dat &= 0<<(7-i); // Clear bit (7 - i)
		else{
			dat |= 1<<(7-i); // Set bit (7 - i)
			while(DPin){ // Wait until pin goes low
				k++;
				if(k>100){
					time_out = 0;
					return 0;
				}
				__delay_us(1);
			}
		}
	}
	return dat;
}

char beginDHT (){
	DPinDir = 0; // Configure connection pin as output
	DPin &= 0; // Connection pin output low
	__delay_ms(25);
	DPin = 1; // Connection pin output high
	__delay_us(30);
	DPinDir = 1; // Configure connection pin as input
	__delay_us(40);
	if(!DPin){ // Read and test if connection pin is low
		__delay_us(80);
		if(DPin){ // Read and test if connection pin is high
			__delay_us(50);
			return 1;
		}
	}
	return 0;
}

void DHTHandler (){
	unsigned char t_byte1, t_byte2, rh_byte1, rh_byte2, checkSum;
	time_out = 1;
	if(!beginDHT())
		res = 'r'; // errore risposta assente
	rh_byte1 = readDHT();
	rh_byte2 = readDHT();
	t_byte1 = readDHT();
	t_byte2 = readDHT();
	checkSum = readDHT();
	if(time_out)
		res = 't'; //errore di timeout
	if(checkSum!=((rh_byte1+rh_byte2+t_byte1+t_byte2) & 0xFF))
		res = 'c'; //errore di checksum
	humid = rh_byte1;
	humid = (humid<<8)|rh_byte2;
	temp = t_byte1;
	temp = (temp<<8)|t_byte2;
	if(temp>0X80)
		temp -= temp&0X7F; //rendi  negativo
	res = 's'; //successo
}