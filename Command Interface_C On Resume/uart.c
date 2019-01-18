
//#include "adc_intf.h"
#include <stdlib.h>
#include <inttypes.h>
#include <p32xxxx.h>

int uart_write_ready(){
    if((U1STA & 0b1000000000) == 1)
        return 0;
    else
        return 1;
}

int uart_read_ready(){
    if((U1STA & 1) == 0)
        return 0;
    else
        return 1;
               
}

void uart_write(char input){
    while((U1STA & 0x200)){
        
    }
    U1TXREG = input;
    
}

char uart_read(){
    char data = 0;
    while((U1STA & 1) == 0){
       
    }
    data = U1RXREG;
    return data;
}

void uart_write_nb(char input){
        U1TXREG = input;
}

char uart_read_nb(){
    char data = 0;
    if(uart_read_ready){
        data = U1RXREG;
        return data;
    }
    else
        return 0;
        
}


void uart_init(){
    ANSELA = 0;
    ANSELB = 0;
    TRISA = 0b1111;//set pin9 to input, pin2 to output
    U1RXR = 0x0;//UART in => RPA2, pin9
    RPA0R = 0x1;//UART out => RPA0, pin2
    U1BRG = 260; //40000000/(16 * 9600) - 1
    U1STA = U1STA | 0x1400;//URXEN
//    U1STA = U1STA | 0b1000000000;//UTXEN
    U1MODE = U1MODE & 0b0111; //set slow mode
    U1MODE = U1MODE | 0x8000;//ON
    
}

void uart_write_string(char* input){
    char* c = input;
    int i = 0;
    for(i = 0; i < 64 && *c; i++){
//        delay_ms(50);
        uart_write(*c);
        c++;
    }
}
//
//void delay_ms(uint16_t input){
//    T1CON = 0x8030;
//    TMR1 = 0;
//    uint16_t delay = (40000 * input)/ 256;
//    while(TMR1 < delay){
//        //wait
//    }
//    TMR1 = 0;
//}

