//#include"function.h"
//
//unsigned char jar_present;
//unsigned char olp_present;
//
//unsigned char Red_solid;
//unsigned char Red_Blink;
//unsigned char White_solid;
//unsigned int jar_val;
//unsigned int olp_val;
//void Inputs_Task(void)
//{
//  Tick_Wait();                /* one sample pair per ~1 ms */
//  if(ADC_Read(OLP_CH) >= 2048)
//  {
//  	jar_present = 0;
//  	olp_present = 1;
//  }
//  else if(ADC_Read(JAR_CH) >= 2048)
//  {
//  	jar_present = 1;
//  	olp_present = 0;
//  }
//  if(olp_present == 1)
//  {
//  	if(jar_present == 1)
//  	{
//  		White_solid = 1;
//  		if(olp_present == 0)
//  		{
//  			jar_present = 0;
//  			
//  			Red_Blink = 1;
//  		}
//  	}
//  	else
//  	{
//  		Red_solid = 1;
//  	}
//  }
//  else
//  {
//  	Red_Blink = 1;
//  }
//				
//}
//
//void Led_Task(void)
//{
//	if(Red_solid == 1)
//	{
//		LED_SetAll(0, 0, 0);
//		LED_SetAll(0, 255, 0); 
//	}
//	else if(White_solid == 1)
//	{
//		LED_SetAll(0, 0, 0);
//		LED_SetAll(255, 255, 255);
//	}
//	else if(Red_Blink == 1)
//	{
//		LED_SetAll(0, 0, 0);
//		LED_SetAll(0, 0, 255);
//	}
//			
//}


#include "function.h"

/* Actual definitions: exactly one copy in the project */
unsigned char jar_present = 0U;
unsigned char olp_present = 0U;


static unsigned char led_state = LED_OFF;
static unsigned char previous_state = 255U;
static unsigned char previous_output = 255U;

static unsigned char blink_on = 1U;
static unsigned int blink_counter = 0U;

void Inputs_Task(void)
{
    unsigned int jar_adc;
    unsigned int olp_adc;

    /* Wait for one timer event */
    Tick_Wait();

    /* Read both channels independently */
    jar_adc = ADC_Read(JAR_CH);
    olp_adc = ADC_Read(OLP_CH);

    /*
     * Assumption:
     * ADC >= threshold means input is active.
     */
    if (jar_adc >= ADC_THRESHOLD)
    {
        jar_present = 1U;
    }
    else
    {
        jar_present = 0U;
    }

    if (olp_adc >= ADC_THRESHOLD)
    {
        olp_present = 1U;
    }
    else
    {
        olp_present = 0U;
    }
}

void Led_Task(void)
{
    unsigned char output;

    /* Determine the required LED state */
    if (olp_present != 0U)
    {
        if (jar_present != 0U)
        {
            led_state = LED_WHITE_SOLID;
        }
        else
        {
            led_state = LED_RED_SOLID;
        }
    }
    else
    {
        led_state = LED_RED_BLINK;
    }

    /* Reset blink timing when state changes */
    if (led_state != previous_state)
    {
        previous_state = led_state;
        blink_counter = 0U;
        blink_on = 1U;
    }

    /* Select actual output */
    if (led_state == LED_WHITE_SOLID)
    {
        output = LED_WHITE_SOLID;
    }
    else if (led_state == LED_RED_SOLID)
    {
        output = LED_RED_SOLID;
    }
    else if (led_state == LED_RED_BLINK)
    {
        blink_counter++;

        if (blink_counter >= BLINK_TICKS)
        {
            blink_counter = 0U;
            blink_on = !blink_on;
        }

        if (blink_on != 0U)
        {
            output = LED_RED_SOLID;
        }
        else
        {
            output = LED_OFF;
        }
    }
    else
    {
        output = LED_OFF;
    }

    /* Update the strip only when output changes */
    if (output != previous_output)
    {
        previous_output = output;

        if (output == LED_WHITE_SOLID)
        {
            LED_SetAll(255U, 255U, 255U);
        }
        else if (output == LED_RED_SOLID)
        {
            LED_SetAll(0u, 255u, 0U);
        }
        else
        {
            LED_SetAll(0U, 0U, 0U);
        }
    }
}



