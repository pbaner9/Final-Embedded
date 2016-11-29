#include "motor.h"
#include "motor_timers.h"
#include "queue.h"
#include "motor_timers.h"
#include "peripheral/oc/plib_oc.h"

#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000   
#define START_VALUE  9000



void MOTOR_Initialize ( void )
{
    //forcing to open up port up for output (apparently Harmony has a problem setting this port to output)
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);

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
    
//    motorsData.myTimer = xTimerCreate("Timer",(5/portTICK_PERIOD_MS),pdTRUE,(void*)2,timerCallbackFunction);
//    xTimerStart(motorsData.myTimer, 0);
//    motorsData.timerCount = 0; // Sets the value for Increment to increase once the timer is Created and Started
//    if(xTimerStart(motorsData.myTimer,10) != pdFAIL)  // Checks if Timer starts, if it does not will output Error Message
//    {
//        dbgOutputLoc('E');
//    }
//    
    timerInitialize();
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    DRV_TMR0_Start();
    
    PLIB_TMR_Period16BitSet(TMR_ID_2,600);
}

void MOTOR_Tasks ( void )
{

    switch ( motorsData.state )  /* Check the application's current state. */
    {
        case MOTOR_STATE_INIT:    /* Application's initial state. */
        {
            bool appInitialized = true;
            motorsData.direction = 'F';
            if (appInitialized)
            {
                //motorsData.direction = 'L';  //Remove
                //motorsData.state = MotorReceiveCommand;
                motorsData.state = MotorMain;
            }  break;
        }
        case MotorReceiveCommand:
        {
            char receivedchar;
            if(!uxQueueMessagesWaitingFromISR(motorsData.myQueue) == 0)
            {
                xQueueReceive(motorsData.myQueue,&receivedchar,portMAX_DELAY);
                motorsData.direction = receivedchar;
                motorsData.state = MotorMain; 
            }
        }
        case MotorMain:
        {
            if (motorsData.direction == 'L')
            {
                // Move to State for Left Direction
                motorsData.state = MotorLeft;
            }
            else if (motorsData.direction == 'R')
            {
                // Move to State for Right Direction
                motorsData.state = MotorRight;
            }
            else if (motorsData.direction == 'F')
            {
                // Move to State for Forward Direction
                motorsData.state = MotorForward;
            }
            else if (motorsData.direction == 'B')
            {
                // Move to State for Backwards Direction
                motorsData.state = MotorBackward;
            }
            else if (motorsData.direction == 'S')
            {
                // Move to State for Stopping the Vehicle
                motorsData.state = MotorStop;
            }
        }
        case MotorLeft:
        {
            motorsData.timerCount= 0;
            while (motorsData.timerCount < 330 )
            {
                LeftMotorControl(true);
                RightMotorControl(false);
                //moveleft();      
            }
            
            motorsData.state = MotorRight; 
            //break;
        }
        case MotorRight:
        {
            motorsData.timerCount= 0;
            while (motorsData.timerCount < 330 )
            {
                LeftMotorControl(false);
                RightMotorControl(true);
                moveright();
            }
            motorsData.state = MotorBackward; 
            //break;
        }
        case MotorBackward:
        {
            motorsData.timerCount= 0;
//            while (motorsData.timerCount < 330 )
//            {
//                moveback();
//                LeftMotorControl(false);
//                RightMotorControl(false);
//            }
            LeftMotorControl(false);
            RightMotorControl(false);
            //motorsData.state = MotorForward; 
          //break;
        }      
        case MotorForward:
        {
            motorsData.timerCount= 0;
//            while (motorsData.timerCount < 630 )
//            {
//                LeftMotorControl(true);
//                RightMotorControl(true);
//                //moveforward();
//            }
            LeftMotorControl(true);
            RightMotorControl(true);
            //motorsData.state = MotorStop; 
            //break;
        }
            
        case MotorStop:
        {
            motorsData.timerCount= 0;
            while (motorsData.timerCount < 630 )
            {
                stopmotor();       
            }
            stopmotor();
            motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
            ///break;
        }

 
        case MOTOR_STATE_SERVICE_TASKS:
        {
            stopmotor();
            break;
        }
        default:  /* TODO: Handle error in application's state machine. */
        {
            stopmotor();
            break;
        }
    }
}

void LeftMotorControl(bool movement)
{   //Pin 78 // RG1  // Which is pin 34 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);  
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);  
 
    }
}

void RightMotorControl (bool movement)
{   //Pin 4 // RC14  // Which is pin 4 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);  
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);  
 
    }
}
// Stopping both motors
void stopmotor() { 
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    PLIB_OC_PulseWidth16BitSet(0, 0);
    PLIB_OC_PulseWidth16BitSet(1, 0); 
}

void moveright()
{
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable(RIGHT_OSCILLATOR);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    
    PLIB_OC_PulseWidth16BitSet(0, 2000);
    PLIB_OC_PulseWidth16BitSet(1, 2000); 
}

void moveleft()
{
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    PLIB_OC_PulseWidth16BitSet(0, 2000);
    PLIB_OC_PulseWidth16BitSet(1, 2000);
    
}

void moveforward()
{
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    
    PLIB_OC_PulseWidth16BitSet(0, 2000);
    PLIB_OC_PulseWidth16BitSet(1, 2000);
}


void moveback() //NEEDS TO BE TESTED
{
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1);
    PLIB_OC_PulseWidth16BitSet(0, 2000);
    PLIB_OC_PulseWidth16BitSet(1, 2000);
}

 

/*******************************************************************************
 End of File
 */
