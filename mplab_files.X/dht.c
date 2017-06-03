#include <htc.h>
#include "dht.h"

#define _XTAL_FREQ 16000000
// Connection pin between PIC18F4550 and DHT22 sensor
#define Data_Pin = PORTBbits.RB0                       // Pin mapped to PORTB.0
#define Data_Pin_Direction = TRISBbits.TRISB0             // Pin direction mapped to TRISB.0

char message1[] = "Temp = 00.0 C";
char message2[] = "RH   = 00.0 %";

short Time_out;

char beginDHT (){
	TRISBbits.TRISB0 = 0; // Configure connection pin as output
	PORTBbits.RB0 = 0; // Connection pin output low
	__delay_ms(25);
	PORTBbits.RB0 = 1; // Connection pin output high
	__delay_us(30);
	TRISBbits.TRISB0 = 1; // Configure connection pin as input
	__delay_us(40);
	if(!PORTBbits.RB0){ // Read and test if connection pin is low
		__delay_us(80);
		if(PORTBbits.RB0){ // Read and test if connection pin is high
			__delay_us(50);
			return 1;
		}
	}
	return 0;
}

char readDHT (){
	unsigned char i, k, dat = 0; // k is used to count 1 bit reading duration
	if(!Time_out) return 0;
	for(i = 0; i<8; i++){
		k=0;
		while(!PORTBbits.RB0){ // Wait until pin goes high
			k++;
			if(k>100){
				Time_out = 0;
				return 0;
			}
			__delay_us(1);
		}
		__delay_us(30);
		if(!PORTBbits.RB0) dat &= 0<<(7-i); // Clear bit (7 - i)
		else{
			dat |= 1<<(7-i); // Set bit (7 - i)
			while(PORTBbits.RB0){ // Wait until pin goes low
				k++;
				if(k>100){
					Time_out = 0;
					return 0;
				}
				__delay_us(1);
			}
		}
	}
	return dat;
}

char DHTHandler (unsigned short* RH, short* Tempf){
	unsigned short Temp;
	char t_byte1, t_byte2, rh_byte1, rh_byte2, checkSum;
	Time_out = 1;
	if(!beginDHT()) return 'r'; // If there is response from sensor
	rh_byte1 = readDHT(); // read RH byte1
	rh_byte2 = readDHT(); // read RH byte2
	t_byte1 = readDHT(); // read T byte1
	t_byte2 = readDHT(); // read T byte2
	checkSum = readDHT(); // read checksum
	if(Time_out) return 't';
	if(checkSum!=((rh_byte1+rh_byte2+t_byte1+t_byte2) & 0xFF)) return 'c';
	*RH = rh_byte1;
	*RH = (*RH<<8)|rh_byte2;
	Temp = t_byte1;
	Temp = (Temp<<8)|t_byte2;

	if(Temp>0X8000){
		*Tempf = -(Temp&0X7FFF);
		//make negative
	}
	return 's';
}