/**
 * TMR6 Generated Driver File
 *
 * @file tmr6.c
 * 
 * @ingroup  tmr6
 * 
 * @brief This file contains the API implementation for the TMR6 module.
 *
 * @version Driver Version 2.0.0
 *
 * @version Package Version 2.0.0
 */
/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include <xc.h>
#include "../tmr6.h"

static void (*TMR6_PeriodMatchCallback)(void);
static void TMR6_DefaultPeriodMatchCallback(void);

/**
  Section: TMR6 APIs
*/

void TMR6_Initialize(void)
{
    // Set TMR6 to the options selected in the User Interface
    PR6 = 0xF9;            // Period 0.032; Frequency 7812.5Hz; Count 249
    TMR6 = 0x0;
    TMR6_PeriodMatchCallback = TMR6_DefaultPeriodMatchCallback;
    
    PIR2bits.TMR6IF = 0;
    T6CON = (3 << _T6CON_T6CKPS_POSN)   // T6CKPS 1:64
        | (1 << _T6CON_TMR6ON_POSN)   // TMR6ON on
        | (15 << _T6CON_T6OUTPS_POSN);  // T6OUTPS 1:16
}

void TMR6_Deinitialize(void)
{
    T6CONbits.TMR6ON = 0;
    PIR2bits.TMR6IF = 0;	   
    PIE2bits.TMR6IE = 0;
    T6CON = 0x0;
    PR6 = 0xFF;
    TMR6 =0x0;
}

void TMR6_Start(void)
{   
    T6CONbits.TMR6ON = 1;
}

void TMR6_Stop(void)
{   
    T6CONbits.TMR6ON = 0;
}

uint8_t TMR6_CounterGet(void)
{
    return TMR6;
}

void TMR6_CounterSet(uint8_t counterValue)
{  
    TMR6 = counterValue;
}

void TMR6_PeriodSet(uint8_t periodCount)
{
    PR6 = periodCount;
}

uint8_t TMR6_PeriodGet(void)
{
    return PR6;
}

uint8_t TMR6_MaxCountGet(void)
{
    return TMR6_MAX_COUNT;
}


void TMR6_PeriodMatchCallbackRegister(void (* CallbackHandler)(void))
{
   TMR6_PeriodMatchCallback = CallbackHandler;
}

static void TMR6_DefaultPeriodMatchCallback(void)
{
    // Default callback function
}

bool TMR6_PeriodMatchStatusGet(void)
{
    return PIR2bits.TMR6IF;
}

void TMR6_PeriodMatchStatusClear(void)
{
    PIR2bits.TMR6IF = 0;
}

void TMR6_Tasks(void)
{
    if(1U == PIR2bits.TMR6IF)
    {
        if(NULL != TMR6_PeriodMatchCallback)
        {
            TMR6_PeriodMatchCallback();
        }
        PIR2bits.TMR6IF = 0;
    }
}
