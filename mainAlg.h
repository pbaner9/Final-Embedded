#ifndef _MAINALG_H
#define _MAINALG_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "debug.h"

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

typedef enum
{
	/* Application's state machine's initial state. */
	MAINALG_STATE_INIT=0,
    Error_Test_1,
    Error_Test_2,
    Error_Test_3,
	MAINALG_STATE_SERVICE_TASKS,

} MAINALG_STATES;

typedef struct
{
    QueueHandle_t myQueue;
    MAINALG_STATES state;
    
} MAINALG_DATA;

MAINALG_DATA mainalgData;

void MAINALG_Initialize ( void );
void MAINALG_Tasks( void );


#endif /* _MAINALG_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif

