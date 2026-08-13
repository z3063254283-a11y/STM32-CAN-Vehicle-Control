#ifndef __MYKEY_H
#define __MYKEY_H

#include "stm32f10x.h"
#include "Delay.h"

void MyKey_Init(void);
uint8_t Key_Get(void);

#endif
