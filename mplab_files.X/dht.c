#include <htc.h>
#include <dht.h>
#define _XTAL_FREQ 16000000

// Connection pin between PIC18F4550 and DHT22 sensor
#define Data_Pin = PORTBbits_t.RB0                       // Pin mapped to PORTB.0
#define Data_Pin_Direction = TRISBbits_t.TRISB0             // Pin direction mapped to TRISB.0

char message1[] = "Temp = 00.0 C";
char message2[] = "RH   = 00.0 %";

short Time_out;

void startSignal (){
	TRISBbits_t.TRISB0 = 0; // Configure connection pin as output
	PORTBbits_t.RB0 = 0; // Connection pin output low
	delay_ms(25);
	PORTBbits_t.RB0 = 1; // Connection pin output high
	delay_us(30);
	TRISBbits_t.TRISB0 = 1; // Configure connection pin as input
}

char checkResponse (){
	delay_us(40);
	if(!PORTBbits_t.RB0){ // Read and test if connection pin is low
		delay_us(80);
		if(PORTBbits_t.RB0){ // Read and test if connection pin is high
			delay_us(50);
			return 1;
		}
	}
	return 0;
}

char readDHT (){
	unsigned int8 i, k, dat=0; // k is used to count 1 bit reading duration
	if(Time_out) return 0;
	for(i = 0; i<8; i++){
		k=0;
		while(!PORTBbits_t.RB0){ // Wait until pin goes high
			k++;
			if(k>100){
				Time_out = 1;
				return 0;
			}
			delay_us(1);
		}
		delay_us(30);
		if(!PORTBbits_t.RB0) dat &= 0<<(7-i); // Clear bit (7 - i)
		else{
			dat |= 1<<(7-i); // Set bit (7 - i)
			while(PORTBbits_t.RB0){ // Wait until pin goes low
				k++;
				if(k>100){
					Time_out = 1;
					return 0;
				}
				delay_us(1);
			}
		}
	}
	return dat;
}

char DHTHandler (unsigned short* RH, short* Temp){
	unsigned char T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum;
	Time_out = 0;
	startSignal();
	if(!checkResponse()) return 'r'; // If there is response from sensor
	RH_byte1 = readDHT(); // read RH byte1
	RH_byte2 = readDHT(); // read RH byte2
	T_byte1 = readDHT(); // read T byte1
	T_byte2 = readDHT(); // read T byte2
	Checksum = readDHT(); // read checksum
	if(!Time_out) return 't';
	if(CheckSum!=((RH_Byte1+RH_Byte2+T_Byte1+T_Byte2) & 0xFF))return 'c';
	*RH = RH_byte1;
	*RH = (*RH<<8)|RH_byte2;
	*Temp = T_byte1;
	*Temp = (*Temp<<8)|T_byte2;

	if(*Temp>0X8000){
		*Temp = *Temp&0X7FFF;
		//make negative
	}
	else message1[6] = ' ';
	message1[7] = (*Temp/100)%10+48;
	message1[8] = (*Temp/10)%10+48;
	message1[10] = *Temp%10+48;
	message2[7] = (*RH/100)%10+48;
	message2[8] = (*RH/10)%10+48;
	message2[10] = *RH%10+48;
	message1[11] = 223; // Degree symbol
}