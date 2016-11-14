#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "queue.h"
#include "timers.h"
    
#define EXAMPLE_CONSTANT 0

    void dbgOutputVal (unsigned char outVal);
    void dbgOutputLoc(unsigned char outVal);
    void sendThroughQueue(char charToSend, QueueHandle_t test);


#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
