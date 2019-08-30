#ifndef __TIME_H
#define __TIME_H

#include<core_cm3.h>
#include<time.h>

#define t 2*72000000

void SysTick_Handler(void);
static __INLINE double SysTick_Config(t*72000000);
double readrt(double i);
#endif