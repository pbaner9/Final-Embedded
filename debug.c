#include "debug.h"

void dbgOutputVal (unsigned char outVal)
{
   PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_E, 0xFF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, outVal); 
}
void dbgOutputLoc(unsigned char outVal)
{
   PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_A, 0xFF);
   PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_A, outVal); 
}

void sendThroughQueue(char charToSend, QueueHandle_t test)
{
    //xQueueSend( test, &charToSend, portMAX_DELAY);
    xQueueSend( test, &charToSend, 0);
    dbgOutputVal(charToSend);
}

/* *****************************************************************************
 End of File
 */
