#include "function.h"

void Tb0_Init(void)
{
    _psc0r = 0x01;                  /* fPSC0 = fSYS/4        */
    _tb0c  = 0x04;                  /* 2^12 / 4 MHz = 1.024 ms */
    _tb0f  = 0;
    _tb0e  = 1;
    _tb0on = 1;
}