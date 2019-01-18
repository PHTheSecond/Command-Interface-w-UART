#include "analog.h"

//void test_analog_init(){
//    ANSELB = 0xE;
//}

void test_analog(uint16_t pinNum){
    char c = 0;
    uint8_t buffer[64];  
    analog_in_init();
    sprintf(buffer, "\r\n");
    uart_write_string(buffer);
    while(1){
        c = uart_read_nb();
        if(c == 'q'){
            sprintf(buffer, "\r\nquit analog input test\r\n\n");
            uart_write_string(buffer);
            break;
        }  
        sprintf(buffer, "analog_in value: %d\r\n", analog_in_read(pinNum - 48));
        uart_write_string(buffer);
    }
}
