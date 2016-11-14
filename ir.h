#ifndef _IR_H
#define _IR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

typedef enum
{
	/* Application's state machine's initial state. */
	IR_STATE_INIT=0,
	IR_STATE_SERVICE_TASKS,
    IR_STATE_SERVICE_TASKS_1,
    IR_STATE_SERVICE_TASKS_2,

	/* TODO: Define states used by the application state machine. */

} IR_STATES;

typedef struct
{
    /* The application's current state */
    IR_STATES state;
    int potValue;
    int ledMask;
    bool dataReady;
    
    char convertedValue;

} IR_DATA;

IR_DATA irData;

void IR_Initialize ( void );
void IR_Tasks( void );


#endif /* _IR_H */

#ifdef __cplusplus
}
#endif

