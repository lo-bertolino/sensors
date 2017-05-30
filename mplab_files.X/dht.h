#ifndef DHT22
#define DHT22

void startSignal();
char checkResponse();
char readDHT();
char DHTHandler(short* RH, short* Temp);
#endif