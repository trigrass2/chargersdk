/**
* @file taskerror.c
* @brief
* @author rgw
* @version v1.0
* @date 2017-01-22
*/
#include "taskcreate.h"
#include "taskerror.h"

void vTaskEVSEError(void *pvParameters)
{
    while(1)
    {
#if DEBUG_TASK
        xprintf("%s\n", TASKNAME_EVSEError);
#endif
        vTaskDelay(1000);
    }
}