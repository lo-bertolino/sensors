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

//#define TEST_SPI 1
#define TEST_DHT 1

#define _XTAL_FREQ 16000000 //frequenza quarzo, per impostazione del delay
#define DHTPin PORTCbits.RC0
#define DHTPinDir TRISCbits.TRISC0
#define UID 1 //ID sensore

#ifndef TEST_SPI
#    ifndef TEST_DHT
#        include "wl_module.h"
#        include "nRF24L01.h"
#    endif //DHT
#endif //SPI

__CONFIG (FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_ON);
#define _XTAL_FREQ 16000000


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
#else
#    if TEST_SPI

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
#    endif

//inizializza uC

	while(1){
		int temp, umid;

		//nRF transmission

	}
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




