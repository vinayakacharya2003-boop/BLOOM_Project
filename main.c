#include<stdio.h>
#include "function.h"
void init_config()
{
    MCU_Clock_Init();

    GPIO_Init();
    ADC_Init();
    LED_Reset();
    Tick_Init();

}
void main()
{
	init_config();
	while(1)
	{
		GCC_CLRWDT();
        Inputs_Task();  
        Led_Task();	
	}
}