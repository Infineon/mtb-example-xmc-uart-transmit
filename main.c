/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: UART Transmit Example
*              for ModusToolbox. This code example demonstrates simple UART
*              communication by printing a "Hello World" message on a terminal
*              and blinking an LED. 
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2021, Infineon Technologies AG
* All rights reserved.                        
*                                             
* Boost Software License - Version 1.0 - August 17th, 2003
* 
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
* 
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*                                                                              
*****************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include "xmc_gpio.h"
#include "xmc_uart.h"

/*******************************************************************************
* Defines
*******************************************************************************/
/* Declarations for LED toggle and UART transmission timing */
#define TICKS_PER_SECOND        1000
#define TICKS_WAIT              1000

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of array to store the message to be transmitted */
const uint8_t message[] = "Hello world!!\r\n";
const uint8_t clear_screen[] = "\x1b[2J\x1b[;H";

/*******************************************************************************
* Function Name: SysTick_Handler
********************************************************************************
* Summary:
* Function called by system timer every millisecond. It toggles an LED and sends
* the message via UART.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void SysTick_Handler(void)
{
    static uint32_t ticks = 0;
    uint32_t index;

    ticks++;
    if (ticks == TICKS_WAIT)
    { 
        /* Toggle the LED */
        XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

        for (index = 0; index < sizeof(message) - 1; index++)
        {
            /* Send a message via UART periodically */
            XMC_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, message[index]);
        }
        ticks = 0;
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It performs the following tasks:
*  1. Initial setup of device.
*  2. Starts the UART peripheral
*  3. Enables the system timer
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{    
    cy_rslt_t result;
    uint32_t index;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Start the UART peripheral*/ 
    XMC_UART_CH_Start(CYBSP_DEBUG_UART_HW);

    /* Send a message to clear the screen */
    for (index = 0; index < sizeof(clear_screen) - 1; index++)
    {
        XMC_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, clear_screen[index]);
    }

    /* Enable system timer */
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);

    while(1)
    {
        /* Infinite loop */
    }
}

/* [] END OF FILE */