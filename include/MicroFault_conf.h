/**
 * @file MicroFault_conf.h
 * @author https://github.com/xfp23
 * @brief Configuration file for MicroFault module.
 * @version 0.1
 * @date 2025-12-01
 *
 * @copyright Copyright (c) 2025
 */

#ifndef __MICROFAULT_CONF_H__
#define __MICROFAULT_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif //! __cplusplus


#define MICROFAULT_VERSION_MAJOR    "0.0.0.1"

/* -------------------------------------------------------------------------- */
/*                              Fault Code Configuration                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Bit width of fault code.
 * Supported values: 8, 16, 32.
 * Affects MicroFault_FaultCode_t typedef.
 */
#define MICROFAULT_FAULTCODE_BIT_WIDTH      8u


/* -------------------------------------------------------------------------- */
/*                            Fault Table Configuration                        */
/* -------------------------------------------------------------------------- */

/**
 * @brief Maximum number of faults that the system can store.
 */
#define MICROFAULT_MAXNUMBER_OF_FAULTS      32u

/**
 * @brief Number of user-defined faults.
 * This is for your custom fault table entries.
 */
#define MICROFAULT_NUMBER_OF_USER_FAULTS    18u


/* -------------------------------------------------------------------------- */
/*                            Fault Level Ordering                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Severity order mode.
 *
 * 0: Normal order  
 *      - Higher number = more severe  
 *      - e.g., Level 3 > Level 2 > Level 1
 *
 * 1: Reversed order  
 *      - Lower number = more severe  
 *      - e.g., Level 1 > Level 2 > Level 3
 *
 * Used to adapt projects where severity numbering rules differ.
 */
#define MICROFAULT_LEVEL_ORDER_REVERSED     1u


#ifdef __cplusplus
}
#endif //! __cplusplus

#endif //! __MICROFAULT_CONF_H__
