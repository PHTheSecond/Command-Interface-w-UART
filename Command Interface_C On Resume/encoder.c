#include "encoder.h"
#define NOTMOVING 0
#define CW 1
#define CCW 2
#define inputA ((PORTB & 0b1))
#define inputB ((PORTB & 0b10) >> 1)


void test_encoder_init(){
    TRISB = TRISB | 0b11;
    ANSELB = 0;
}

void test_encoder(){
    uint8_t buffer[64];
    //0 => not moving, 1 => CW, 2 => CCW
    uint8_t direction = NOTMOVING;
    char c = 0;
    int prevA = 1;
    int prevB = 1;
    int count = 0;
    sprintf(buffer, "\r\n");
    uart_write_string(buffer);
    while(1){
        c = uart_read_nb();
        if(c == 'q'){
            sprintf(buffer, "\r\nquit encoder test\r\n\n");
            uart_write_string(buffer);
            break;
        } 
        else if(inputA && inputB){
            if(direction != NOTMOVING && count > 3){
                if(direction == CW){
                    sprintf(buffer, "a+\r\n");
                    uart_write_string(buffer);
                }
                else{
                    sprintf(buffer, "a-\r\n");
                    uart_write_string(buffer);
                }      
            }   
            prevA = 1;
            prevB = 1;
            count = 0;
            direction = NOTMOVING;
            
            

        }
        else if((inputA != prevA) || inputB != prevB){
                
                if(direction == NOTMOVING && !(inputA) && inputB && prevA != 0 && prevB != 0){
                    direction = CW;
                }
                else if(direction == NOTMOVING && !(inputB) && inputA && prevA != 0 && prevB != 0){
                    direction = CCW;
                }
                count++;
                prevA = inputA;
                prevB = inputB;
        }
        else{
            //do nothing
        }
    }
}
