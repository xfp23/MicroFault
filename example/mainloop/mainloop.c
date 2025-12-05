#include "MicroFault.h"
#include "stdio.h"

MicroFault_FaultInfo_t FaultInfo = {0};

const MicroFault_FaultTableEntry_t faultTable[] = {
    {.code = 0x01, .level = MICROFAULT_LEVEL_SERIOUS},
    {.code = 0x02, .level = MICROFAULT_LEVEL_GENERAL},
    {.code = 0x03, .level = MICROFAULT_LEVEL_MINOR},

};

int main(void)
{
    MicroFault_Status_t status = 0;

    MicroFault_Init();

    status = MicroFault_RegisterFault(faultTable, sizeof(faultTable) / sizeof(faultTable[0]));

    MicroFault_SetFault(0x02);

    while (1)
    {
    MicroFault_TimerHandler(&FaultInfo);
    printf("fault code : 0x%02X, level: %d\r\n", FaultInfo.code, FaultInfo.level);
    MicroFault_ClearFault(0x02);
    }
    
}