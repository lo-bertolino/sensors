/* File:   main.c
 * Author: Lorenzo Bertolino
 * Created on 20 december 2016, 9:31
 */
#pragma config PLLDIV = 1, CPUDIV = OSC1_PLL2, USBDIV = 1, FOSC = HS
#pragma config FCMEN = OFF, IESO = OFF, PWRT = ON, BOR = ON, BORV = 3
#pragma config VREGEN = OFF, WDT = OFF, WDTPS = 32768, CCP2MX = ON
#pragma config PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON, STVREN = ON, LVP = ON
#pragma config ICPRT = OFF, XINST = OFF, CP0 = OFF, CP1 = OFF, CP2 = OFF
#pragma config CP3 = OFF, CPB = OFF, CPD = OFF, WRT0 = OFF, WRT1 = OFF
#pragma config WRT2 = OFF, WRT3 = OFF, WRTB = OFF, WRTC = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTRB = OFF

#include <htc.h>
#include <stdlib.h>
#include "spi.h"
#include "nRF24L01.h"
#include "wl_module.h"

#define _XTAL_FREQ 16000000 //frequenza quarzo, per impostazione del delay
#define DHTPin PORTCbits.RC0
#define DHTPinDir TRISCbits.TRISC0
#define UID 1 //ID sensore

//nRF
#define nRF_use

struct {
	unsigned char ID;
	char Status;
	short temp;
	unsigned short humid;
	unsigned short counter;
} dati;
unsigned char payload[wl_module_PAYLOAD];

//dht
//#define DHT_use
char time_out, res;
unsigned short humid;
short temp;

char readDHT()
{
	unsigned char i, k, dat = 0; // k is used to count 1 bit reading duration
	if (!time_out) return 0;
	for (i = 0; i < 8; i++) {
		k = 0;
		while (!DHTPin) { // Wait until pin goes high
			k++;
			if (k > 100) {
				time_out = 0;
				return 0;
			}
			__delay_us(1);
		}
		__delay_us(30);
		if (!DHTPin) dat &= 0 << (7 - i); // Clear bit (7 - i)
		else {
			dat |= 1 << (7 - i); // Set bit (7 - i)
			while (DHTPin) { // Wait until pin goes low
				k++;
				if (k > 100) {
					time_out = 0;
					return 0;
				}
				__delay_us(1);
			}
		}
	}
	return dat;
}

char beginDHT()
{
	DHTPinDir = 0; // Configure connection pin as output
	DHTPin &= 0; // Connection pin output low
	__delay_ms(25);
	DHTPin = 1; // Connection pin output high
	__delay_us(30);
	DHTPinDir = 1; // Configure connection pin as input
	__delay_us(40);
	if (!DHTPin) { // Read and test if connection pin is low
		__delay_us(80);
		if (DHTPin) { // Read and test if connection pin is high
			__delay_us(50);
			return 1;
		}
	}
	return 0;
}

void DHTHandler()
{
	unsigned char t_byte1, t_byte2, rh_byte1, rh_byte2, checkSum;
	time_out = 1;
	if (!beginDHT())
		res = 'r'; // errore risposta assente
	rh_byte1 = readDHT();
	rh_byte2 = readDHT();
	t_byte1 = readDHT();
	t_byte2 = readDHT();
	checkSum = readDHT();
	if (time_out)
		res = 't'; //errore di timeout
	if (checkSum != ((rh_byte1 + rh_byte2 + t_byte1 + t_byte2) & 0xFF))
		res = 'c'; //errore di checksum
	humid = rh_byte1;
	humid = (humid << 8) | rh_byte2;
	temp = t_byte1;
	temp = (temp << 8) | t_byte2;
	if (temp > 0X80)
		temp -= temp & 0X7F; //rendi  negativo
	res = 0; //successo
}

//inizializza uC

void Init()
{
	//debug
	TRISB = 0;
	//nRF setup
	wl_module_init();
	wl_module_tx_config(0); //wl_module_TX_NR_0
	//interrupts
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
	//reset dati
	dati.ID = UID; //id
	dati.counter = 0;
}

int main()
{
	Init();
	while (1) {
#ifdef DHT_use
		DHTHandler();
		dati.Status = res;
		dati.humid = humid;
		dati.temp = temp;
#else
		dati.Status = 's'; //stato = teSt
		dati.humid = 80; //solo per test
		dati.temp = 25; //solo per test
#endif
#ifdef nRF_use
		dati.counter++; //questo no
		payload = dati;
		wl_module_send(payload, wl_module_PAYLOAD);
#endif
		__delay_ms(2000);
	}
}




