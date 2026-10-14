#include "function.h"

void ADC_Init(void)
{
    /* pin-shared function: PB0 = AN0 (PBS0[1:0]=10), PB1 = AN1 (PBS0[3:2]=10) */
    _pbs01 = 1;  _pbs00 = 0;
    _pbs03 = 1;  _pbs02 = 0;
 

    _sains3 = 0; _sains2 = 0; _sains1 = 0; _sains0 = 0;  /* external input     */

    _sacks2 = 1; _sacks1 = 0; _sacks0 = 0;               /* fSYS/16 = 1 us     */

    _adrfs  = 1;                    /* SADOH = D[11:8], SADOL = D[7:0]         */

    _savrs1 = 0; _savrs0 = 0;       /* reference = AVDD                        */

    _adcen  = 1;                    /* enable ADC                              */
    GCC_DELAY(200);                 /* power-on settling (~50 us)              */
}

unsigned int ADC_Read(unsigned char ch)
{
    _sacs3 = (ch >> 3) & 1;         /* select channel AN<ch>                   */
    _sacs2 = (ch >> 2) & 1;
    _sacs1 = (ch >> 1) & 1;
    _sacs0 =  ch       & 1;

    _start = 0; _start = 1; _start = 0;   /* 0->1->0 starts conversion         */
    while (_adbz)
    {
        GCC_CLRWDT();
    }
    return ((unsigned int)_sadoh << 8) | _sadol;
}

/* 1 ms time base, polled (no interrupt): fPSC0 = fSYS/4, 2^12 counts = 1.024 ms */
void Tick_Init(void)
{
    _psc0r = 0x01;
    _tb0c  = 0x04;
    _tb0f  = 0;
    _tb0on = 1;
}

void Tick_Wait(void)
{
    while (!_tb0f)
    {
        GCC_CLRWDT();
    }
    _tb0f = 0;
}