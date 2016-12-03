#include "motor.h"
#include "motor_timers.h"
#include "queue.h"
#include "motor_timers.h"
#include "peripheral/oc/plib_oc.h"

#define LEFT_OSCILLATOR  OC_ID_2
#define RIGHT_OSCILLATOR OC_ID_1
#define PWM_MAX    10000 
#define PWM_TURN   4000
#define PWM_STOP   0
#define START_VALUE  9000
#define ENCODER_VALUE 27


void MOTOR_Initialize ( void )
{
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
    DRV_TMR0_Start();
    // Creation of Software Timer     
    timerInitialize();    
    // Initalization of OC and Motors
    initalizeOCandMotors();
    // Initalization of Motor Values
    initializeMotorValues(&motorsData.leftMotor);
    initializeMotorValues(&motorsData.rightMotor);
}

void MOTOR_Tasks ( void )
{

    switch ( motorsData.state )  /* Check the application's current state. */
    {
        case MOTOR_STATE_INIT:    /* Application's initial state. */
        {
            bool appInitialized = true;
            //motorsData.direction = 'L';
            if (appInitialized)
            {
                //motorsData.direction = 'L';  //Remove
                //motorsData.state = MotorReceiveCommand;
                motorsData.state = MotorReceiveCommand;
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
            if(motorsData.timerCount == 0)
            {
                while (motorsData.timerCount < 27 )
                {
                    moveleft();      
                }
            }
//            else {
//                motorsData.state = MotorMain;  // Moves back to main so nothing gets stuck.
//            }
            stopmotor();
            motorsData.state = MotorReceiveCommand; 
            //break;
        }
        case MotorRight:
        {
            motorsData.timerCount= 0;
            if(motorsData.timerCount == 0)
            {
                while (motorsData.timerCount < 27 )
                {
                    moveright();      
                }
            }
            stopmotor();
            motorsData.state = MotorReceiveCommand; 
            //break;
        }
        case MotorBackward:
        {
            motorsData.timerCount= 0;
            if(motorsData.timerCount == 0)
            {
                while (motorsData.timerCount < 10 )
                {
                    moveback();      
                }
            }
            //stopmotor();
            motorsData.state = MotorReceiveCommand; 
          //break;
        }      
        case MotorForward:
        {
            motorsData.timerCount= 0;
            if(motorsData.timerCount == 0)
            {
                while (motorsData.timerCount < 10 )
                {
                    moveforward();      
                }
            }
            //stopmotor();
            motorsData.state = MotorReceiveCommand; 
            //break;
        }
            
        case MotorStop:
        {
            motorsData.timerCount= 0;
            if(motorsData.timerCount == 0)
            {
                while (motorsData.timerCount < 15 )
                {
                    stopmotor();      
                }
            }
            //stopmotor();
            //motorsData.direction = 'L';
            motorsData.state = MotorReceiveCommand;  // Sends it back to main state to receive next direction
            ///break;
        }

 
        case MOTOR_STATE_SERVICE_TASKS:
        {
            stopmotor();
            break;
        }
    }
}
void initializeMotorValues (motorValues* motor)
{
    motor->maxValue = PWM_MAX;
    motor->stopValue = PWM_STOP;
    motor->turnValue = PWM_TURN;
    motor->ExpectedEncoder = ENCODER_VALUE;
    motor->encoderValue = 0;
    motor->oldEncoderValue = 0;
}

void incrementLeftMotor()
{
    motorsData.leftMotor.encoderValue++;
}
void incrementRightMotor()
{
    motorsData.rightMotor.encoderValue++;
}
void initalizeOCandMotors()
{
    // Initalizes the OC Drivers
    PLIB_OC_Enable(LEFT_OSCILLATOR);
    PLIB_OC_Enable (RIGHT_OSCILLATOR);
    // Start the OC drivers
    DRV_OC0_Start();
    DRV_OC1_Start();
    //DRV_TMR1_Start();
    // Attempts to Change Timer Setting to better control the motors
    PLIB_TMR_Period16BitSet(TMR_ID_2,600);
    //forcing to open up port up for output (apparently Harmony has a problem setting this port to output)
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    // Sets the pins to One
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
    PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1);
    // Initalizes the Pulse Modulation to be Zero and Turned off until a command is given
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, PWM_STOP);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, PWM_STOP);  
}

void LeftMotorControl(bool movement)
{   //Pin 78 // RG1  // Which is pin 34 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        //PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1,0);
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        //PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1,1);
 
    }
}


void RightMotorControl (bool movement)
{   //Pin 4 // RC14  // Which is pin 4 on Motor Shield 
    if(movement == true)
    {   // This will clear the direction pin and tell the motor to move forward
        //PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14); 
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14,0);
    }
    else
    {   // This will set a 1 to direction pin and Reverse the Motor
        //PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14);
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14,1);
 
    }
}
// Stopping both motors
void stopmotor() { 
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, PWM_STOP);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, PWM_STOP); 
}

void moveright()
{
    LeftMotorControl(false);
    RightMotorControl(true);
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.maxValue);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.turnValue);
}

void moveleft()
{
    LeftMotorControl(true);
    RightMotorControl(false);
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.turnValue);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.maxValue);
    
}

void moveforward()
{
    LeftMotorControl(true);
    RightMotorControl(true);
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.maxValue);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.maxValue);
}


void moveback() //NEEDS TO BE TESTED
{
    LeftMotorControl(false);
    RightMotorControl(false);
    PLIB_OC_PulseWidth16BitSet(LEFT_OSCILLATOR, motorsData.leftMotor.maxValue);
    PLIB_OC_PulseWidth16BitSet(RIGHT_OSCILLATOR, motorsData.rightMotor.maxValue);
}

 

/*******************************************************************************
 End of File
 */
