#ifndef __MYUART_H
#define __MYUART_H

#include "stm32f10x.h"
#include <stdio.h> 

void MyUART_Init(uint32_t baud);
void MyUART_SendByte(uint8_t data);
void MyUART_SendString(char *str);

#endif
