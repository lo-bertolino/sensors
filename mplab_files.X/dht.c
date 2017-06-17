/*#include <htc.h>
#include "dht.h"

#define _XTAL_FREQ 16000000
// Connection pin between PIC18F4550 and DHT22 sensor
#define DPin PORTCbits.RC0            // Pin mapped to PORTB.0
#define DPinDir TRISCbits.TRISC0      // Pin direction mapped to TRISB.0

char readDHT (){
	unsigned char i, k, dat = 0; // k is used to count 1 bit reading duration
	if(!time_out) return 0;
	for(i = 0; i<8; i++){
		k=0;
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
}*/

/*
byte addresses[][6] = {"1Node", "2Node"};

radio.begin ();
radio.enableAckPayload ();
radio.enableDynamicPayloads ();
radio.openWritingPipe (addresses[0]);
radio.openReadingPipe (1, addresses[1]);
radio.startListening (); // Start listening

//loop

byte pipeNo;
while(radio.available(&pipeNo)){
	byte gotByte;
	radio.read(&gotByte, 1);
	temperature[0] = dht.getTemperature();
	temperature[1] = dht.getHumidity();
	radio.writeAckPayload(pipeNo, temperature, sizeof (temperature));
}
dht.readSensor ();
delay (2000);
//*/