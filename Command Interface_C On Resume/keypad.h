#ifndef KEYPAD_H
#define	KEYPAD_H
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>

void test_keypad_init();
void test_keypad();
void setRows(uint8_t rowNum);
uint8_t readCols();

#endif	/* KEYPAD_H */

