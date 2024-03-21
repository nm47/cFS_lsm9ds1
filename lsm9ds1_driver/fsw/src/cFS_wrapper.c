#include "cFS_wrapper.h"
// #include "cFS_wrapper_cmds.h"

LSM9DS1_DRIVER_Data_t LSM9DS1_DRIVER_Data;

void LSM9DS1_DRIVER_Main(void)
{
    // CFE_ES_PerfLogEntry(LSM9DS1_DRIVER_PERF_ID);

    int32            status = LSM9DS1_DRIVER_Init();
    CFE_SB_Buffer_t *SBBufPtr;

    if (status != CFE_SUCCESS)
    {
        LSM9DS1_DRIVER_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    // Enter Runloop
    while (CFE_ES_RunLoop(&LSM9DS1_DRIVER_Data.RunStatus) == true)
    {
        // Check for command packets
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, LSM9DS1_DRIVER_Data.CommandPipe, CFE_SB_POLL);

        if (status == CFE_SB_NO_MESSAGE)
        {
            continue;
        }

        // pass command to driver
    }
}

int32 LSM9DS1_DRIVER_Init(void)
{
    CFE_ES_WriteToSysLog("LSM9DS1_DRIVER_INIT");

    int32 status = CFE_SUCCESS;

    /* Zero out the global data structure */
    memset(&LSM9DS1_DRIVER_Data, 0, sizeof(LSM9DS1_DRIVER_Data));

    LSM9DS1_DRIVER_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    strncpy(LSM9DS1_DRIVER_Data.PipeName, "LSM9DS1_DRIVER_CMD_PIPE", sizeof(LSM9DS1_DRIVER_Data.PipeName));
    LSM9DS1_DRIVER_Data.PipeName[sizeof(LSM9DS1_DRIVER_Data.PipeName) - 1] = 0;
    LSM9DS1_DRIVER_Data.PipeDepth = 32; // TODO move this to a constant wherever NASA does that kind of thing

    // Create Software Bus message pipe.
    status = CFE_SB_CreatePipe(&LSM9DS1_DRIVER_Data.CommandPipe, LSM9DS1_DRIVER_Data.PipeDepth,
                               LSM9DS1_DRIVER_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Lsm9ds1 handler: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    return status;
}
