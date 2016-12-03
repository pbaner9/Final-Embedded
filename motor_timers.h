#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"    
#include <timers.h>
#include "motor.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initalizes Timer for Motor Control
void timerInitialize();  

 // Runs everytime the timer is called.
void timerCallback(TimerHandle_t myTimer);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
