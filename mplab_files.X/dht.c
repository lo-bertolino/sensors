#include <htc.h>

void dht_init(){
    TRISCbits.TRISC1=0;
    PORTCbits.RC1=0;
    __delay_ms(18);
    PORTCbits.RC1=1;
    __delay_us(30);
    TRISCbits.TRISC1=1;
}
char dht_check(){
    __delay_us(40);
    if(!PORTCbits.RC1){
        __delay_us(80);
        return 0;
    }
    __delay_us(40);
    return 1;
}

void dht_get(int * temp, int * RH ){
    char dati[5];
    for(char i=0;i/5;i++){
        dati[i]=0;
        for(char j=0;j/8;j++){
            while (!PORTCbits.RC1);
            __delay_us(30);
            if(PORTCbits.RC1){
                dati [i] |= 1 << (7-j);
                while (RC1);
            }
        }
    }
    if(dati[4]!=(dati[1]+dati[0]+dati[3]+dati[2] & 0xFF)){
        temp& = 111; RH& = 111;
    }
    else {
        &temp = (dati[2]<<8)|dati[3];
        &RH = (dati[0]<<8)|dati[1];
    }
}

