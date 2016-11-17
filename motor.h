#ifndef _MOTOR_H
#define _MOTOR_H

#include <stdint.h>
#include "FreeRTOS.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "timers.h"
#include "system_config.h"
#include "system_definitions.h"

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

typedef enum
{
    /* Application's state machine's initial state. */
    MOTOR_STATE_INIT=0,
    MOTOR_STATE_SERVICE_TASKS,
    MotorReceiveCommand,
    MotorMain,
    MotorForward,
    MotorBackward,
    MotorLeft,
    MotorRight,
    MotorStop,

} MOTOR_STATES;

typedef struct
{
    /* The application's current state */
    MOTOR_STATES state;
    QueueHandle_t myQueue;
    TimerHandle_t myTimer;  // Creates a Timer for Motor Control
    /* TODO: Define any additional data used by the application. */
    char message[2];
    char type;
    int timerCount;
    char direction;
    uint8_t data; //direction data
    
} MOTOR_DATA;
//this is the global struct to add to motor queue
MOTOR_DATA motorsData;

void stopmotor(void);
void moveright(void);
void moveleft(void);
void moveforward(void);
void moveback(void);

void MOTOR_Initialize ( void );
void MOTOR_Tasks( void );



#endif /* _MOTOR_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

