// Configure clocks
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20 //40 MHz
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2 // PB 40 MHz
#pragma config FWDTEN = OFF,  FSOSCEN = OFF, JTAGEN = OFF

#include "analog.h"
#include "encoder.h"
#include "keypad.h"
#include <inttypes.h>


void print_help(){
    uint8_t buffer[64];
    sprintf(buffer, "\r\nHelp:\r\n?: help\r\na{n}: analog input test on pin n\r\n");
    uart_write_string(buffer);
    sprintf(buffer, "e: encoder test\r\ni: system version\r\nm: keypad test\r\n\n");
    uart_write_string(buffer);
}

void main(){
    uint8_t buffer[64];
    uart_init();
    
    char c = 0;
    char input [3] = "  ";//at pos 0, variable index is 1
    uint8_t index = 0;
    uint16_t pinNum = -1;
    
    loop:
    sprintf(buffer, "Command > ");
    uart_write_string(buffer);
    
    while(1){
        c = uart_read();
        //CR
        if(c != 13){
            //backspace
            if(c == 8 && index > 0){
                uart_write(8);
                uart_write(32);
                uart_write(8);
                index = index - 1;
                input[index] = ' ';
            }
            else if(c != 8 && index < 2){
                uart_write(c);
                index = index + 1;
                input[index - 1] = c;
            }
        }
        else{
            if(input[0] == '?' && input[1] == ' '){
                //reset
                index = 0;
                input[0] = ' ';
                input[1] = ' ';               
                print_help();
                goto loop;
            }
            else if(input[0] == 'a' && input[1] > 47 && input[1] < 58){
                pinNum = input[1];
                //reset
                index = 0;
                input[0] = ' ';
                input[1] = ' ';    
//                test_analog_init();
                test_analog(pinNum);               
                goto loop; 
            }
            else if(input[0] == 'e' && input[1] == ' '){
                index = 0;
                input[0] = ' ';
                input[1] = ' ';
                test_encoder_init();
                test_encoder();
                goto loop;
            }
            else if(input[0] == 'm' && input [1] == ' '){
                index = 0;
                input[0] = ' ';
                input[1] = ' ';
                test_keypad_init();
                test_keypad();
                goto loop;
            }
            else if(input[0] == 'i' && input[1] == ' '){
                index = 0;
                input[0] = ' ';
                input[1] = ' ';
                sprintf(buffer, "\r\nFeng & Phillip ECE414 Lab5 Version 1\r\n\n");
                uart_write_string(buffer);
                goto loop;
            }
                
        }
    }
    
}


