#include "MicroFault.h"
#include "string.h"

static volatile MicroFault_Object_t MicroFault_Obj = {0};

MicroFault_Handle_t const MicroFault_Handle = &MicroFault_Obj;

MicroFault_Status_t MicroFault_Init()
{
    MicroFault_Handle->count = MICROFAULT_NUMBER_OF_USER_FAULTS;

    return MICROFAULT_STATUS_OK;
}

MicroFault_Status_t MicroFault_RegisterFault(const MicroFault_FaultTableEntry_t *table, size_t size)
{
    if (table == NULL || size == 0 || size > MICROFAULT_MAXNUMBER_OF_FAULTS)
    {
        return MICROFAULT_STATUS_INVALID_PARAM;
    }

    for (size_t i = 0; i < size; i++)
    {
        MicroFault_Handle->table[i].code = table[i].code;
        MicroFault_Handle->table[i].level = table[i].level;
        MicroFault_Handle->table[i].enabled = false;
    }
    return MICROFAULT_STATUS_OK;
}

MicroFault_Status_t MicroFault_SetFault(MicroFault_FaultCode_t code)
{
    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        if (MicroFault_Handle->table[i].code == code)
        {
            MicroFault_Handle->table[i].enabled = true;
            return MICROFAULT_STATUS_OK;
        }
    }
    return MICROFAULT_STATUS_NOT_FOUND;
}

MicroFault_Status_t MicroFault_ClearFault(MicroFault_FaultCode_t code)
{
    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        if (MicroFault_Handle->table[i].code == code)
        {
            MicroFault_Handle->table[i].enabled = false;
            return MICROFAULT_STATUS_OK;
        }
    }
    return MICROFAULT_STATUS_NOT_FOUND;
}

MicroFault_Status_t MicroFault_ClearALL()
{
    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        MicroFault_Handle->table[i].enabled = false;
    }
    return MICROFAULT_STATUS_OK;
}

MicroFault_Status_t MicroFault_SetALL()
{
    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        MicroFault_Handle->table[i].enabled = true;
    }
    return MICROFAULT_STATUS_OK;
}

MicroFault_Status_t MicroFault_TimerHandler(MicroFault_FaultInfo_t *info)
{
    if (info == NULL)
    {
        return MICROFAULT_STATUS_INVALID_PARAM;
    }

#if MICROFAULT_LEVEL_ORDER_REVERSED == 0 // Normal Order (3=Serious, 1=Minor, higher value is more serious)
    MicroFault_Level_t highestLevel = MICROFAULT_LEVEL_NONE;
    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        if (MicroFault_Handle->table[i].enabled == true && MicroFault_Handle->table[i].level > highestLevel)
        {
            highestLevel = MicroFault_Handle->table[i].level;
        }
    }

    if (highestLevel == MICROFAULT_LEVEL_NONE)
    {
        info->level = MICROFAULT_LEVEL_NONE;
        info->code = 0;
        return MICROFAULT_STATUS_OK;
    }

#elif MICROFAULT_LEVEL_ORDER_REVERSED == 1 // Reversed Order (1=Serious, 3=Minor, lower value is more serious)

    MicroFault_Level_t highestLevel = (MicroFault_Level_t)(MICROFAULT_LEVEL_MINOR + 1);
    bool faultFound = false;

    for (size_t i = 0; i < MicroFault_Handle->count; i++)
    {
        if (MicroFault_Handle->table[i].enabled == true)
        {
            faultFound = true;
            if (MicroFault_Handle->table[i].level < highestLevel)
            {
                highestLevel = MicroFault_Handle->table[i].level;
            }
        }
    }

    if (!faultFound) // 如果没有找到任何活跃故障
    {
        info->level = MICROFAULT_LEVEL_NONE;
        info->code = 0;
        return MICROFAULT_STATUS_OK;
    }

#else
#error "Unsupported MICROFAULT_LEVEL_ORDER_REVERSED value"
#endif

    size_t start = MicroFault_Handle->index;

    for (size_t i = start; i < MicroFault_Handle->count; i++)
    {
        if (MicroFault_Handle->table[i].enabled)
        {
            MicroFault_Handle->index = i + 1;
            if (MicroFault_Handle->index >= MicroFault_Handle->count)
                MicroFault_Handle->index = 0;

            info->level = highestLevel;
            info->code = MicroFault_Handle->table[i].code;
            return MICROFAULT_STATUS_OK;
        }
    }

    for (size_t i = 0; i < start; i++)
    {
        if (MicroFault_Handle->table[i].enabled)
        {
            MicroFault_Handle->index = i + 1;
            if (MicroFault_Handle->index >= MicroFault_Handle->count)
                MicroFault_Handle->index = 0;

            info->level = highestLevel;
            info->code = MicroFault_Handle->table[i].code;
            return MICROFAULT_STATUS_OK;
        }
    }

    // 没找到
    info->level = MICROFAULT_LEVEL_NONE;
    info->code = 0;
    return MICROFAULT_STATUS_NOT_FOUND;
}
