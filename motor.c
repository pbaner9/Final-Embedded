#include "motor.h"
#include "timers.h"
#include "peripheral/oc/plib_oc.h"

#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000   
#define START_VALUE  9000

void timerCallbackFunction(TimerHandle_t myTimer)
{
    /* The number of times this timer has expired is saved as the
    timer's ID.  Obtain the count. */
    ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
}

void MOTOR_Initialize ( void )
{
    //forcing to open up port up for output (apparently Harmony has a problem setting this port to output)
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);   

    /* Place the App state machine in its initial state. */
    motorsData.state = MOTOR_STATE_INIT;
    motorsData.myQueue = xQueueCreate( 12, sizeof( char ) );
    if( motorsData.myQueue == 0 )
    {
        // Queue was not created
        // Do something for Error Output
        // Need to make a function
        //break;
    }
    // Creation of Software Timer
    appData.myTimer = xTimerCreate("krc",(50/portTICK_PERIOD_MS),pdTRUE,(void*)0,timerCallbackFunction);
    if(xTimerStart(appData.myTimer,10) != pdFAIL)  // Checks if Timer starts, if it does not will output Error Message
    {
        dbgOutputLoc('E');
    }
}

void MOTOR_Tasks ( void )
{
    switch ( motorsData.state )  /* Check the application's current state. */
    {
        case MOTOR_STATE_INIT:    /* Application's initial state. */
        {
            bool appInitialized = true;
            if (appInitialized)
            {
                motorsData.state = Motor_Test_1;
            }  break;
        }
        
        // This is a test where the stop symbol is not sent
        case Motor_Test_1:
        {                 
            sendThroughQueue('~', uartData.motorthread); // 0
            sendThroughQueue('A', uartData.motorthread); // 1
            sendThroughQueue('M', uartData.motorthread); // 2
            sendThroughQueue('L', uartData.motorthread); // 3
            sendThroughQueue(0, uartData.motorthread); // 4 -- 0 for now
            sendThroughQueue(0, uartData.motorthread); // 5 -- 0 for now
            sendThroughQueue(0, uartData.motorthread); // 6 -- 0 for now
            sendThroughQueue('R', uartData.motorthread); // 7
            sendThroughQueue(0, uartData.motorthread); // 8 -- 0 for now
            sendThroughQueue(0, uartData.motorthread); // 9 -- 0 for now
            sendThroughQueue(0, uartData.motorthread); // 10 -- 0 for now
            sendThroughQueue('*', uartData.motorthread); // 11
            break;
        }

        case MOTOR_STATE_SERVICE_TASKS:
        {
        
            break;
        }
        default:  /* TODO: Handle error in application's state machine. */
        {
            break;
        }
    }
}

// Stopping both motors
void stopmotor(void) {
    
    //pin config
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable(RIGHT_OSCILLATOR);
    
    //left motor
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    
    //right motor
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);

}

void moveright(void)
{
    //pin config
    PLIB_OC_Disable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    
     //left motor
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    
    //right motor 
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    
}

void moveleft(void)
{
    //pin config
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Disable(RIGHT_OSCILLATOR);
    
     //left motor
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    
    //right motor 
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    
    }

void moveforward(void)
{
     //left motor
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    
    //right motor 
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
}


void moveback(void) //NEEDS TO BE TESTED
{
    //left motor
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    
    //right motor 
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
}

 

/*******************************************************************************
 End of File
 */
