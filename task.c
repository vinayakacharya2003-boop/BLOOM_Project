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

    static unsigned int jar_peak = 0U;
    static unsigned int olp_peak = 0U;
    static unsigned char window = 0U;

    /* Wait for one timer event */
    Tick_Wait();

    /* Read both channels independently */
    jar_adc = ADC_Read(JAR_CH);
    olp_adc = ADC_Read(OLP_CH);

    /* Hold the highest value seen in this window */
    if (jar_adc > jar_peak)
    {
        jar_peak = jar_adc;
    }
    if (olp_adc > olp_peak)
    {
        olp_peak = olp_adc;
    }

    window++;

    /* ~25 ms window = more than one 50 Hz cycle (20 ms) */
    if (window >= 25U)
    {
        window = 0U;

        if (jar_peak >= ADC_THRESHOLD)
        {
            jar_present = 1U;
        }
        else
        {
            jar_present = 0U;
        }

        if (olp_peak >= ADC_THRESHOLD)
        {
            olp_present = 1U;
        }
        else
        {
            olp_present = 0U;
        }

        jar_peak = 0U;
        olp_peak = 0U;
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
            LED_SetAll(255, 255, 255);
            //LED_SetAll(100, 150, 200);
        }
        else if (output == LED_RED_SOLID)
        {
            LED_SetAll(0, 255, 0);
        }
        else
        {
            LED_SetAll(0, 0, 0);
        }
    }
}



