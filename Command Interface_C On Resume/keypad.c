#include "keypad.h"

#define ROW1EN 0b1110000000000000 //RB11
#define ROW2EN 0b1100100000000000//RB13
#define ROW3EN 0b1010100000000000//RB14
#define ROW4EN 0b0110100000000000 //RB15
#define COL1 !!(PORTB & 0b10000000)//RB7
#define COL2 !!(PORTB & 0b0000000100000000)//RB8
#define COL3 !!(PORTB & 0b0000001000000000)//RB9
#define COL4 !!(PORTB & 0b0000000000100000)//RB5
//#define COL4 1
//switch c and r: working!

void test_keypad_init(){
//    0b000011110000000
//    CNPUB = 0x3A0;
    //0b0000001110100000
    TRISB = 0x03A0;
    ANSELB = 0;
}

void test_keypad(){
    uint8_t buffer[64];
    char c = 0;
    int rowCount = 1;
    //key values:
    //1  2 3  10
    //4  5 6  11
    //7  8 9  12
    //14 0 15 13 
    //-2: not pressed
    //default: -1
    int key = -1;
    int level2Pulse = 0;
    sprintf(buffer, "\r\n");
    uart_write_string(buffer);
    while(1){
        c = uart_read_nb();
        if(c == 'q'){
            sprintf(buffer, "\r\nquit keypad test\r\n\n");
            uart_write_string(buffer);
            break;
        }
        setRows(rowCount);
        switch(rowCount){
            case 1:
                if(readCols() == 0b0001)
                    key = 1;
                else if(readCols() == 0b0010)
                    key = 2;
                else if(readCols() == 0b0100)
                    key = 3;
                else if(readCols() == 0b1000)
                    key = 10;               
                else if(COL1 & COL2 & COL3 & COL4){
                    key = -2;
                    level2Pulse = 0;
                    rowCount++;
                }
                else
                    key = -1;               
                break;
            case 2:
                if(readCols() == 0b0001)
                    key = 4;   
                else if(readCols() == 0b0010)
                    key = 5;           
                else if(readCols() == 0b0100)
                    key = 6;
                else if(readCols() == 0b1000)
                    key = 11;
                else if(COL1 & COL2 & COL3 & COL4){
                    key = -2;
                    level2Pulse = 0;
                    rowCount++;
                }
                else
                    key = -1;               
                break;
            case 3:
                if(readCols() == 0b0001)
                    key = 7;
                else if(readCols() == 0b0010)
                    key = 8;
                else if(readCols() == 0b0100)
                    key = 9;
                else if(readCols() == 0b1000)
                    key = 12;
                else if(COL1 & COL2 & COL3 & COL4){
                    key = -2;
                    level2Pulse = 0;
                    rowCount++;
                }
                else
                    key = -1;                
                break;
            case 4:
                if(readCols() == 0b0001)
                    key = 14;
                else if(readCols() == 0b0010)
                    key = 0;
                else if(readCols() == 0b0100)
                    key = 15;
                else if(readCols() == 0b1000)
                    key = 13;
                else if(COL1 & COL2 & COL3 & COL4){
                    key = -2;
                    level2Pulse = 0;
                    rowCount = 1;
                }
                else
                    key = -1;
                break;
                
        }
        if(key < 14 && key > 9 && level2Pulse == 0){
            sprintf(buffer, "%c\r\n", key + 55);
            uart_write_string(buffer);
            level2Pulse = 1;
            key = -1;
        }     
        else if(key == 14 && level2Pulse == 0){
            sprintf(buffer, "*\r\n");
            uart_write_string(buffer);
            level2Pulse = 1;
            key = -1;
        }
        else if(key == 15 && level2Pulse == 0){
            sprintf(buffer, "#\r\n");
            uart_write_string(buffer);
            level2Pulse = 1;
            key = -1;
        }
        else if(key >= 0 && key < 10 && level2Pulse == 0){
            sprintf(buffer, "%d\r\n", key);
            uart_write_string(buffer);
            level2Pulse = 1;
            key = -1;
        }
//        setRows(2);
    }
}

//{ROW4, ROW3, ROW2, ROW1}
void setRows(uint8_t rowNum){
    if(rowNum == 1){
//        PORTB = ROW1EN; 
        PORTB = ROW1EN;
    }
    else if(rowNum == 2){
        PORTB = ROW2EN;
    }
    else if(rowNum == 3){
        PORTB = ROW3EN;
    }
    else if(rowNum = 4){
        PORTB = ROW4EN;
    }
    else{
        PORTB = 0;
    }
}

uint8_t readCols(){
//    uint8_t buffer[64];
//    sprintf(buffer, "%d, %d, %d, %d\r\n", COL1, COL2, COL3, COL4);
//    uart_write_string(buffer);
    if(!COL1 & COL2 & COL3 & COL4){
        return 0b0001;
    }
    else if(COL1 & !COL2 & COL3 & COL4){
        return 0b0010;
    }
    else if(COL1 & COL2 & !COL3 & COL4){
        return 0b0100;
    }
    else if(COL1 & COL2 & COL3 & !COL4){
        return 0b1000;
    }
    else{
        return -1;
    }
}
