#include "HT66F3195.h"
#include "function.h"

/*=========================================================================
  TIMING BUDGET  (fSYS = 16 MHz)

  Holtek 8-bit core: 1 instruction cycle = 4 x fSYS clocks
                                         = 4 / 16 MHz = 250 ns
  set/clr pin bit = 1 cycle, NOP = 1 cycle, taken branch = 2 cycles.

  LED datasheet (page 8):
      "0" : T0H = 0.3 us (0.25~0.35)   T0L = 0.9 us
      "1" : T1H = 0.9 us (0.85~0.95)   T1L = 0.3 us
      T (bit period) >= 1.2 us, LOW time of a bit < 20 us
      Reset : LOW > 200 us

  With a 250 ns step the only possible HIGH times are 250,500,750,1000 ns:
      "0" HIGH = 1 cycle  = 250 ns   (spec 300 +-50   -> just inside)
      "1" HIGH = 4 cycles = 1000 ns  (spec 900 +-50   -> 50 ns over, closest)
  The LOW time is whatever the loop overhead gives (>1 us, far below 20 us),
  which the datasheet allows.  This also keeps the bit period >= 1.2 us.

  Pin high time = number of instruction cycles between the "set" and
  the following "clr" instruction.
=========================================================================*/

/* Select a timing profile (see table).  START WITH 1.
   1 : "0" = 250 ns high, "1" = 1000 ns high   (closest to datasheet)
   2 : "0" = 500 ns high, "1" = 1000 ns high   (use if '0' pulses are not detected)
   3 : "0" = 250 ns high, "1" =  750 ns high   (your original timing)        */
#define LED_TIMING_PROFILE   1

#if   (LED_TIMING_PROFILE == 1)
    #define LED_BIT_0()  do{ _pb2 = 1; _pb2 = 0; }while(0)
    #define LED_BIT_1()  do{ _pb2 = 1; GCC_DELAY(3); _pb2 = 0; }while(0)
#elif (LED_TIMING_PROFILE == 2)
    #define LED_BIT_0()  do{ _pb2 = 1; GCC_DELAY(1); _pb2 = 0; }while(0)
    #define LED_BIT_1()  do{ _pb2 = 1; GCC_DELAY(3); _pb2 = 0; }while(0)
#elif (LED_TIMING_PROFILE == 3)
    #define LED_BIT_0()  do{ _pb2 = 1; _pb2 = 0; }while(0)
    #define LED_BIT_1()  do{ _pb2 = 1; GCC_DELAY(2); _pb2 = 0; }while(0)
#endif

/*=========================================================
  RESET / LATCH : line LOW > 200 us
  1200 instruction cycles x 250 ns = 300 us
=========================================================*/
void LED_Reset(void)
{
    _pb2 = 0;
    GCC_DELAY(1200);
}

/*=========================================================
  ms delay : 4000 cycles x 250 ns = 1 ms
=========================================================*/
void Delay_ms(unsigned int ms)
{
    while(ms--)
    {
        GCC_CLRWDT();
        GCC_DELAY(4000);
    }
}

/*=========================================================
  SEND ONE BYTE, MSB FIRST
=========================================================*/
void LED_SendByte(unsigned char data)
{
    unsigned char i;

    for(i = 8; i != 0; i--)
    {
        if(data & 0x80)
        {
            LED_BIT_1();
        }
        else
        {
            LED_BIT_0();
        }
        data <<= 1;
    }
}

/*=========================================================
  SEND ONE LED (24 bits) - datasheet order is R, G, B
=========================================================*/
void LED_SendRGB(unsigned char red,
                 unsigned char green,
                 unsigned char blue)
{
    LED_SendByte(red);
    LED_SendByte(green);
    LED_SendByte(blue);
}

/*=========================================================
  ALL 9 LEDs SAME COLOUR : 216 bits, then reset(latch)
=========================================================*/
void LED_SetAll(unsigned char red,
                unsigned char green,
                unsigned char blue)
{
    unsigned char i;

    unsigned char emi_state;

    emi_state = _emi;          /* save global interrupt state */
    _emi = 0;
    
    for(i = 0; i < LED_COUNT; i++)
    {
        LED_SendRGB(red, green, blue);
    }
    _emi = emi_state;          /* restore before the long reset low */

    LED_Reset();
}

