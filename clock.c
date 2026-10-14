#include "function.h"
/*----MCU CLOCK INITIALIZATION----*/
//void MCU_Clock_Init(void)
//{
//    _hircc = 0x09;      /* HIRC = 16 MHz, HIRCEN = 1 */
//    _scc   = 0x00;      /* fSYS = fH = HIRC, no divider -> 16 MHz */
//
//    GCC_DELAY(20);      /* allow clock switching to complete */
//}

void MCU_Clock_Init(void)
{
	_hircc = 0x09;
    while(_hircf == 0);   /* wait until HIRC stable */
    _scc = 0x00;
}