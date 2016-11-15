#include "motor.h"
#include "timers.h"
#include "peripheral/oc/plib_oc.h"

#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000   
#define START_VALUE  9000

//--------------------------------------------------------------//


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
   
   
  
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void MOTOR_Tasks ( void )

  Remarks:
    See prototype in motor.h.
 */

void MOTOR_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( motorsData.state )
    {
        /* Application's initial state. */
        case MOTOR_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
//                mainalgData.state =  Motor_Test_1;
                motorsData.state = Motor_Test_1;
            }
            break;
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

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
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
