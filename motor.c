#include "motor.h"
#include "timers.h"
#include "peripheral/oc/plib_oc.h"


//--------------------------------------------------------------//

//Global Variables 
#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000   
#define START_VALUE  9000

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

void MOTOR_Initialize ( void )
{
   //forcing to open up port up for output (apparently Harmony has a problem setting this port to output)
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);   

    timerInitialize();
    stopmotor(); //stop motor by default
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
//              mainalgData.state =  Motor_Test_1;
                motorsData.state = Motor_Test_1;
            }
            break;
        }
        
    }
}

/*******************************************************************************
 End of File
 */
