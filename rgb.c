//Thanh Pham

#include "rgb.h"

RGB_DATA rgbData;

void RGB_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    rgbData.state = RGB_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


void RGB_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( rgbData.state )
    {
        /* Application's initial state. */
        case RGB_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                rgbData.state = RGB_STATE_SERVICE_TASKS;
            }
            break;
        }

        case RGB_STATE_SERVICE_TASKS:
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