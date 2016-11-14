#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "debug.h"
#ifdef __cplusplus  // Provide C++ Compatibility
#endif

typedef enum
{
	/* Application's state machine's initial state. */
	UART_STATE_INIT=0,
	UART_STATE_SERVICE_TASKS,
    UART_STATE_SERVICE_TASKS_1,
    UART_STATE_SERVICE_TASKS_2,
    UART_STATE_SERVICE_TASKS_3,
    UART_STATE_SERVICE_TASKS_4,
    UART_STATE_SERVICE_TASKS_5,

} UART_STATES;

typedef struct
{
    /* The application's current state */
    UART_STATES state;
    QueueHandle_t myQueue;
    QueueHandle_t receiveQ;
    QueueHandle_t mainthread;
    QueueHandle_t motorthread;
    QueueHandle_t irthread;
    QueueHandle_t rgbthread;
    
    int mainthreadbool;
    /* TODO: Define any additional data used by the application. */

} UART_DATA;
// Used to call the Queue from other thread
UART_DATA uartData;


void UART_Initialize ( void );

void UART_Tasks( void );


#endif /* _UART_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

