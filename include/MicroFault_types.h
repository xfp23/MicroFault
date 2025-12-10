#ifndef __MICROFAULT_TYPES_H__
#define __MICROFAULT_TYPES_H__

/**
 * @file MicroFault_types.h
 * @author …
 * @brief Global type definitions for MicroFault
 */

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "MicroFault_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* Fault Code Type                                                            */
/* -------------------------------------------------------------------------- */

#if MICROFAULT_FAULTCODE_BIT_WIDTH == 8u
typedef uint8_t  MicroFault_FaultCode_t;
#elif MICROFAULT_FAULTCODE_BIT_WIDTH == 16u
typedef uint16_t MicroFault_FaultCode_t;
#elif MICROFAULT_FAULTCODE_BIT_WIDTH == 32u
typedef uint32_t MicroFault_FaultCode_t;
#else
#error "Unsupported MICROFAULT_FAULTCODE_BIT_WIDTH"
#endif

/* -------------------------------------------------------------------------- */
/* Status                                                                     */
/* -------------------------------------------------------------------------- */

typedef enum
{
    MICROFAULT_STATUS_OK,
    MICROFAULT_STATUS_ERROR,
    MICROFAULT_STATUS_INVALID_PARAM,
    MICROFAULT_STATUS_NOT_FOUND,
} MicroFault_Status_t;

/* -------------------------------------------------------------------------- */
/* Fault Level                                                                */
/* -------------------------------------------------------------------------- */

typedef enum
{

    MICROFAULT_LEVEL_NONE = 0,       // No fault
#if MICROFAULT_LEVEL_ORDER_REVERSED == 0
    MICROFAULT_LEVEL_SERIOUS = 3,    // Serious fault
    MICROFAULT_LEVEL_GENERAL = 2,    // General fault
    MICROFAULT_LEVEL_MINOR = 1,      // Minor fault
#elif MICROFAULT_LEVEL_ORDER_REVERSED == 1
    MICROFAULT_LEVEL_SERIOUS = 1,    // Serious fault
    MICROFAULT_LEVEL_GENERAL = 2,    // General fault
    MICROFAULT_LEVEL_MINOR = 3,      // Minor fault
#else
#error "Unsupported MICROFAULT_LEVEL_ORDER_REVERSED value"
#endif

} MicroFault_Level_t;

/* -------------------------------------------------------------------------- */
/* User Fault Table Entry                                                     */
/* -------------------------------------------------------------------------- */

typedef struct
{
    MicroFault_Level_t      level;
    MicroFault_FaultCode_t  code;
} MicroFault_FaultTableEntry_t;

typedef MicroFault_FaultTableEntry_t MicroFault_FaultInfo_t;
/* -------------------------------------------------------------------------- */
/* Total Fault Table Entry                                                    */
/* -------------------------------------------------------------------------- */

typedef struct
{
    MicroFault_Level_t      level;
    MicroFault_FaultCode_t  code;
    bool                    enabled;
} MicroFault_TotalEntry_t;

/* -------------------------------------------------------------------------- */
/* Config                                                                     */
/* -------------------------------------------------------------------------- */

typedef struct
{
    size_t numberOfFaults;
} MicroFault_Config_t;

/* -------------------------------------------------------------------------- */
/* Fault Object                                                               */
/* -------------------------------------------------------------------------- */

typedef struct
{
    size_t                  count;
    MicroFault_TotalEntry_t table[MICROFAULT_MAXNUMBER_OF_FAULTS];
    size_t                  index;
} MicroFault_Object_t;

typedef MicroFault_Object_t *MicroFault_Handle_t; // Handle type for MicroFault object

#ifdef __cplusplus
}
#endif

#endif // __MICROFAULT_TYPES_H__
