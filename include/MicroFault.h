#ifndef __MICROFAULT_H__
#define __MICROFAULT_H__

/**
 * @file MicroFault.h
 * @author https://github.com/xfp23
 * @brief main include file for MicroFault
 * @version 0.1
 * @date 2025-12-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "MicroFault_types.h"

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

extern MicroFault_Handle_t const MicroFault_Handle;

/**
 * @brief Initialize the MicroFault module.
 * 
 * Must be called once at system startup before using any other MicroFault functions.
 * Sets up the fault management system and initializes the fault table count.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: Initialization successful
 *         - MICROFAULT_STATUS_ERROR: Initialization failed
 */
extern MicroFault_Status_t MicroFault_Init();

/**
 * @brief Register a table of faults with their codes and severity levels.
 * 
 * Copies fault definitions from the provided table into the internal fault management system.
 * All registered faults are initially disabled.
 * 
 * @param table Pointer to an array of fault table entries. Must not be NULL.
 * @param size  Number of entries in the table. Must be > 0 and <= MICROFAULT_MAXNUMBER_OF_FAULTS.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: Faults registered successfully
 *         - MICROFAULT_STATUS_INVALID_PARAM: Invalid parameters (NULL table, size 0, or size exceeds maximum)
 */
extern MicroFault_Status_t MicroFault_RegisterFault(const MicroFault_FaultTableEntry_t *table, size_t size);

/**
 * @brief Enable/activate a specific fault by its code.
 * 
 * Searches the fault table for the specified fault code and marks it as enabled.
 * If the fault is already enabled, this function has no additional effect.
 * 
 * @param code The fault code to activate. Must match a registered fault code.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: Fault enabled successfully
 *         - MICROFAULT_STATUS_NOT_FOUND: No matching fault code found in the table
 */
extern MicroFault_Status_t MicroFault_SetFault(MicroFault_FaultCode_t code);

/**
 * @brief Disable/deactivate a specific fault by its code.
 * 
 * Searches the fault table for the specified fault code and marks it as disabled.
 * If the fault is already disabled, this function has no additional effect.
 * 
 * @param code The fault code to clear. Must match a registered fault code.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: Fault disabled successfully
 *         - MICROFAULT_STATUS_NOT_FOUND: No matching fault code found in the table
 */
extern MicroFault_Status_t MicroFault_ClearFault(MicroFault_FaultCode_t code);

/**
 * @brief Disable all registered faults at once.
 * 
 * Clears all active faults by marking every entry in the fault table as disabled.
 * Useful for system reset or initialization.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: All faults cleared successfully
 */
extern MicroFault_Status_t MicroFault_ClearALL();

/**
 * @brief Enable all registered faults at once.
 * 
 * Activates every fault in the fault table by marking them as enabled.
 * Primarily useful for system testing and diagnostics.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: All faults enabled successfully
 */
extern MicroFault_Status_t MicroFault_SetALL();

/**
 * @brief Query the highest severity fault level among all active faults.
 * 
 * Scans the fault table to find the most severe enabled fault.
 * Returns the severity level of that fault or MICROFAULT_LEVEL_NONE if no faults are active.
 * Typically called periodically from a timer interrupt or main loop.
 * 
 * @param info Pointer to output structure where the highest fault level will be stored.
 *             Must not be NULL.
 * 
 * @return MicroFault_Status_t 
 *         - MICROFAULT_STATUS_OK: Query successful, info contains the highest severity level
 *         - MICROFAULT_STATUS_INVALID_PARAM: NULL pointer passed for info parameter
 */
extern MicroFault_Status_t MicroFault_TimerHandler(MicroFault_FaultInfo_t *info);

/**
 * @brief Get the fault table code Status
 * 
 * @param code fault code
 * @return true fault enable
 * @return false fault disable
 */
extern bool MicroFault_GetFault(MicroFault_FaultCode_t code);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !__MICROFAULT_H__