//#ifndef _FUNCTION_H_
//#define _FUNCTION_H_
//
//#include "HT66F3195.h"
//#include<stdio.h>
//#define LED_COUNT   12
//#define JAR_CH          0
//#define OLP_CH          1
//
//void MCU_Clock_Init(void);
//void GPIO_Init(void);
//void LED_Reset(void);
//void Delay_ms(unsigned int ms);
//
///* NOTE: these prototypes MUST be visible to main.c.
//   LED_SetAll() was commented out in the old header, so main.c called it
//   with no prototype (implicit int arguments) -> wrong parameter passing
//   is possible on the Holtek compiler.                                    */
//void LED_SetAll(unsigned char red, unsigned char green, unsigned char blue);
//void LED_SetOne(unsigned char led_no, unsigned char red, unsigned char green, unsigned char blue);
//
//void LED_SendRGB(unsigned char red, unsigned char green, unsigned char blue);
//void LED_SendByte(unsigned char data);
//
//
///* "Fill" train: LEDs light up one at a time, LED0 first, staying
//   lit, until all 9 are on, then the whole strip switches off and
//   the fill starts again.                                          */
//void LED_TrainFill(unsigned char red,
//                    unsigned char green,
//                    unsigned char blue,
//                    unsigned int  step_delay_ms,
//                    unsigned int  hold_ms,
//                    unsigned int  off_ms);
//                    
///* "Train" chase effect: a bright head pixel with a fading tail,
//   travelling around the 9-LED ring.                                */
//void LED_ShowTrain(unsigned char head_pos,
//                    unsigned char red,
//                    unsigned char green,
//                    unsigned char blue,
//                    unsigned char tail_len);
// 
//void LED_TrainPattern(unsigned char red,
//                       unsigned char green,
//                       unsigned char blue,
//                       unsigned char tail_len,
//                       unsigned int  step_delay_ms);
//                       
//void ExtInt_Init(void);
//void Tb0_Init(void);
//
//unsigned int ADC_Read(unsigned char ch);
//void ADC_Init(void);
//
//#endif


#ifndef FUNCTION_H
#define FUNCTION_H

#include "HT66F3195.h"

/*=========================================================
  CONFIGURATION
=========================================================*/

#define LED_COUNT       12U

/* Verify ADC channel mapping for your PCB */
#define JAR_CH          0U
#define OLP_CH          1U

/* Assumed 12-bit ADC threshold; calibrate on hardware */
#define ADC_THRESHOLD   2048U

/* Assumes one timer tick per millisecond */
#define BLINK_TICKS     500U

/* LED states */
#define LED_OFF         0U
#define LED_RED_SOLID   1U
#define LED_WHITE_SOLID 2U
#define LED_RED_BLINK   3U

/*=========================================================
  SHARED VARIABLES
=========================================================*/

extern unsigned char jar_present;
extern unsigned char olp_present;

/*=========================================================
  MCU INITIALIZATION
=========================================================*/

void MCU_Clock_Init(void);
void GPIO_Init(void);

/*=========================================================
  ADC FUNCTIONS
=========================================================*/

void ADC_Init(void);
unsigned int ADC_Read(unsigned char ch);

/*=========================================================
  TIMER FUNCTIONS
=========================================================*/

void Tick_Init(void);
void Tick_Wait(void);

/*=========================================================
  LED FUNCTIONS
=========================================================*/

void LED_Reset(void);

void LED_SendByte(unsigned char data);

void LED_SendRGB(unsigned char red,
                 unsigned char green,
                 unsigned char blue);

void LED_SetAll(unsigned char red,
                unsigned char green,
                unsigned char blue);

/*=========================================================
  APPLICATION FUNCTIONS
=========================================================*/

void Inputs_Task(void);
void Led_Task(void);

#endif
