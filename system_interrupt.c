/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include <sys/attribs.h>
#include "mainalg.h"
#include "uart.h"
#include "motor.h"
#include "ir.h"
#include "rgb.h"
#include "system_definitions.h"


int i = 0;
char receivedChar;
char recChar;


void IntHandlerDrvAdc(void)
{
    int i;

    irData.dataReady = true;
    
    //Read data before clearing interrupt flag
    for(i=0;i<16;i++)
    {
        irData.potValue += PLIB_ADC_ResultGetByIndex(ADC_ID_1, i);
    }
    irData.potValue = irData.potValue / 16 ;
 
    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
    /* Clear ADC Interrupt Flag */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}




    
void IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}


void IntHandlerDrvUsartInstance0()
{
    dbgOutputVal ('I');
    if (PLIB_INT_SourceFlagGet(INT_ID_0, INT_SOURCE_USART_1_TRANSMIT))//while(!uxQueueMessagesWaitingFromISR(uartData.myQueue) == 0)
    {
        while(!uxQueueMessagesWaitingFromISR(uartData.myQueue) == 0)
        {
            if(PLIB_USART_TransmitterIsEmpty(USART_ID_1))
            {
			   xQueueReceiveFromISR(uartData.myQueue,&receivedChar,NULL);
               dbgOutputVal ('3'); 
               PLIB_USART_TransmitterByteSend(USART_ID_1, receivedChar);
               dbgOutputVal (receivedChar);
            }
        }    
    }
    if (PLIB_INT_SourceFlagGet(INT_ID_0, INT_SOURCE_USART_1_RECEIVE))
    {
        if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_1))
        {
            PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_USART_1_RECEIVE);
            recChar = PLIB_USART_ReceiverByteReceive(USART_ID_1);
            dbgOutputVal (recChar);
        }
    }
   //PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_USART_1_TRANSMIT);
  
    
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_1_TRANSMIT);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_1_RECEIVE);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_1_ERROR);
    
    //DRV_USART_TasksTransmit(sysObj.drvUsart0);
    //DRV_USART_TasksReceive(sysObj.drvUsart0);
    //DRV_USART_TasksError(sysObj.drvUsart0);
}
 
 
 

 

 

 

 

 
 
 
/*******************************************************************************
 End of File
*/

