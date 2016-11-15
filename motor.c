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
    // TODO : Add a count Integer for Ticks.
    // ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
    motorsData.timerCount++;
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
    motorsData.myTimer = xTimerCreate("krc",(50/portTICK_PERIOD_MS),pdTRUE,(void*)0,timerCallbackFunction);
    motorsData.timerCount = 0; // Sets the value for Increment to increase once the timer is Created and Started
    if(xTimerStart(motorsData.myTimer,10) != pdFAIL)  // Checks if Timer starts, if it does not will output Error Message
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
                // motorsData.state = MotorTestCase;  // Uncomment if showing Test case and comment out line below
                motorsData.state = MotorMain;

            }  break;
        }
        //Test Is Moved
        // If required copy this and place back
        // If asked for Error Test Cases
        /*case MotorTestCase:
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
            motorsData.state = MotorMain;
        }
        */
        case MotorMain:
        {                 
            // TODO:
            // Redirect and If statements Propertly
            // Add Code from UART for Server Commands
            
            //Remove
            motorsData.direction = "L";
            //
            if (motorsData.direction == "L")
            {
                // Move to State for Left Direction
                motorsData.state = MotorLeft;
            }
            else if (motorsData.direction == "R")
            {
                // Move to State for Right Direction
                motorsData.state = MotorRight;
            }
            else if (motorsData.direction == "F")
            {
                // Move to State for Forward Direction
                motorsData.state = MotorForward;
            }
            else if (motorsData.direction == "B")
            {
                // Move to State for Backwards Direction
                motorsData.state = MotorBackward;
            }
            else if (motorsData.direction == "S")
            {
                // Move to State for Stopping the Vehicle
                motorsData.state = MotorStop;
            }
            
            break;
        }
        case MotorLeft:
        {
            // THis is where you will call your functions
            // SUDO Code
            // while ([timertickcount]  < 100 ) {  // Do your functions // Move Left };
            motorsData.timerCount= 0;
            while (motorsData.timerCount < 100 )
            {
                moveleft();       
            }
            stopmotor();
            
            //Remove
            motorsData.state = MOTOR_STATE_SERVICE_TASKS; 
            //Remove
            
            //motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
            break;
        }
        case MotorRight:
        {
            // THis is where you will call your functions
            // SUDO Code
            // while ([timertickcount]  < 100 ) {  // Do your functions // Move Left };
            stopmotor();
            motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
            break;
        }
        case MotorBackward:
        {
            // THis is where you will call your functions
            // SUDO Code
            // while ([timertickcount]  < 100 ) {  // Do your functions // Move Left };
            stopmotor();
            motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
            break;
        }      
        case MotorForward:
        {
            // THis is where you will call your functions
            // SUDO Code
            // while ([timertickcount]  < 100 ) {  // Do your functions // Move Left };
            stopmotor();
            motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
            break;
        }
            
        case MotorStop:
        {
            // THis is where you will call your functions
            // SUDO Code
            // while ([timertickcount]  < 100 ) {  // Do your functions // Move Left };
            stopmotor();
            motorsData.state = MotorMain;  // Sends it back to main state to receive next direction
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
