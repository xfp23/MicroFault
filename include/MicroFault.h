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

MicroFault_Status_t MicroFault_Init();

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !__MICROFAULT_H__