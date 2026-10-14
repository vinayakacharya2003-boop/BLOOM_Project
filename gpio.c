#include "function.h"
/*=========================================================
  LED DATA PIN : PB2 (pin 21), GPIO output, idle LOW
=========================================================*/
void GPIO_Init(void)
{
	_pbs05 = 0;         /* PB2 function = GPIO (PB2/STCK not used) */
    _pbs04 = 0;

    _pb2   = 0;         /* latch LOW before enabling output */
    _pbpu2 = 0;         /* no pull-high */
    _pbc2  = 0;         /* PB2 = output */
    _pb2   = 0;
}

