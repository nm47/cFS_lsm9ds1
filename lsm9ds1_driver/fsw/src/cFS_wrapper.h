#ifndef LSM9DS1_DRIVER_H
#define LSM9DS1_DRIVER_H

#include "cfe.h"
#include "cfe_config.h"

// #include "lsm9ds1_driver_perfids.h"

typedef struct
{
    uint8 CmdCounter;
    uint8 ErrCounter;
    /*
    ** Housekeeping telemetry_packet...
    */
    // LSM9DS1_DRIVER_HkTlm_t HkTlm;

    uint32 RunStatus;

    CFE_SB_PipeId_t CommandPipe;
    char            PipeName[CFE_MISSION_MAX_API_LEN];
    uint16          PipeDepth;

} LSM9DS1_DRIVER_Data_t;

extern LSM9DS1_DRIVER_Data_t LSM9DS1_DRIVER_Data;

void  LSM9DS1_DRIVER_Man(void);
int32 LSM9DS1_DRIVER_Init(void);

#endif /* LSM9DS1_DRIVER_H */
