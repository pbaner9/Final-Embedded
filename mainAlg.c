#include "mainalg.h"

void MAINALG_Initialize ( void )
{
    mainalgData.myQueue = xQueueCreate( 120, sizeof( char ) );
    if( mainalgData.myQueue == 0 )
    {
        // Queue was not created
        // Do something for Error Output
        // Need to make a function
        //break;
    }
    mainalgData.state = MAINALG_STATE_INIT;
}

void MAINALG_Tasks ( void )
{
    switch ( mainalgData.state )
    {
        case MAINALG_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized) {
                mainalgData.state =  Error_Test_1;
            } break;
        }
        // This is the test case where a start symbol is not sent
        case Error_Test_1:
        {
            sendThroughQueue('A', uartData.mainthread); // 0
            sendThroughQueue('T', uartData.mainthread); // 1
            sendThroughQueue('A', uartData.mainthread); // 2
            sendThroughQueue('D', uartData.mainthread); // 3
            sendThroughQueue('U', uartData.mainthread); // 4
            sendThroughQueue('L', uartData.mainthread); // 5
            sendThroughQueue('M', uartData.mainthread); // 6
            sendThroughQueue('O', uartData.mainthread); // 7
            sendThroughQueue('M', uartData.mainthread); // 8
            sendThroughQueue('E', uartData.mainthread); // 9
            sendThroughQueue('N', uartData.mainthread); // 10
            sendThroughQueue('*', uartData.mainthread); // 11
            uartData.mainthreadbool = 1;
            //IntHandlerDrvUsartInstance0(uartData.mainthread);
            mainalgData.state = Error_Test_2;
            break;
        }
        // This is a test where the stop symbol is not sent
        case Error_Test_2:
        {
            sendThroughQueue('~', uartData.mainthread); // 0
            sendThroughQueue('T', uartData.mainthread); // 1
            sendThroughQueue('A', uartData.mainthread); // 2
            sendThroughQueue('D', uartData.mainthread); // 3
            sendThroughQueue('U', uartData.mainthread); // 4
            sendThroughQueue('L', uartData.mainthread); // 5
            sendThroughQueue('M', uartData.mainthread); // 6
            sendThroughQueue('O', uartData.mainthread); // 7
            sendThroughQueue('M', uartData.mainthread); // 8
            sendThroughQueue('E', uartData.mainthread); // 9
            sendThroughQueue('N', uartData.mainthread); // 10
            sendThroughQueue('N', uartData.mainthread); // 11
            //IntHandlerDrvUsartInstance0(uartData.mainthread);
            mainalgData.state = Error_Test_3;
            break;
        }
        case Error_Test_3:
        {
            sendThroughQueue('~', uartData.mainthread); // 0
            sendThroughQueue('T', uartData.mainthread); // 1
            sendThroughQueue('A', uartData.mainthread); // 2
            sendThroughQueue('D', uartData.mainthread); // 3
            sendThroughQueue('U', uartData.mainthread); // 4
            sendThroughQueue('L', uartData.mainthread); // 5
            sendThroughQueue('M', uartData.mainthread); // 6
            sendThroughQueue('O', uartData.mainthread); // 7
            sendThroughQueue('E', uartData.mainthread); // 8
            sendThroughQueue('E', uartData.mainthread); // 9
            sendThroughQueue('N', uartData.mainthread); // 10
            sendThroughQueue('*', uartData.mainthread); // 11
            mainalgData.state = MAINALG_STATE_SERVICE_TASKS;
            break;
        }

        case MAINALG_STATE_SERVICE_TASKS:
        {
        
            //break;
        }
        
        default:
        {
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
