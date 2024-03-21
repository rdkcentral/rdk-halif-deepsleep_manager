/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @defgroup Deepsleep_Manager Deep Sleep Manager
 * @{
 * @par Application API Specification
 * Deep sleep is a power saving mode which turns off STB 
 * subsystems such as A/V, HDMI, front panels, HDD etc.
 * Provides API defines the structures and functions for the  Deep Sleep Manager HAL.
 * - The main purpose is to bring down the power consumption based on the actual usage
 * - It can also be triggered in other scenarios such as 
 *   thermal shutdown in case of the temperature is above threshold for certain period.
 * - When the STB goes in a normal power saving mode, it may be woken up 
 *   for scheduled maintenance ina predetermined time.
 */

/**
 * @defgroup Deepsleep_Manager_HAL Deep Sleep Manager HAL
 * @{
 */

/**
 * @defgroup Deepsleep_Mgr_H Deep Sleep Manager Header
 * @{
 */


/**
 * @file deepSleepMgr.h
 *
 * @brief  Deep Sleep Manager HAL Public API
 *
 * This API defines the structures and functions for the  Deep Sleep Manager HAL
 *
 * @par Abbreviations
 * - HAL:     Hardware Abstraction Layer
 * - CPE:     Customer Premises Equipment
 * - IR:      Infra-Red
 * - CEC:     Consumer Electronic Control
 * - LAN:     Local Area Network
 * - WLAN:    Wireless Local Area Network
 * - STB:     Set-top Box
 * - RCU:     Remote Control Unit
 * - STR:     Suspend To RAM
 * - HDMI:    High-Definition Multimedia Interface
 * - GPIO:    General Purpose Input/Output
 * - RCU:     Remote Control Unit
 * - BT:      Bluetooth
 * - RF4CE:   Radio Frequency for Consumer Electronics
 */


#ifndef _IARM_BUS_DEEPSLEEPMGR_H
#define _IARM_BUS_DEEPSLEEPMGR_H

#include "libIARM.h"
#include "libIBusDaemon.h"


#ifdef __cplusplus
extern "C" 
{
#endif

#define IARM_BUS_DEEPSLEEPMGR_NAME 				"DEEPSLEEPMgr"  /*!< Power manager IARM bus name */

typedef enum _DeepSleepStatus_t {
    DeepSleepStatus_Failed = -1,        /*!< Deepsleep operation  failed*/
    DeepSleepStatus_NotStarted = 0,        /*!< Deepsleep operation not started*/
    DeepSleepStatus_InProgress,        /*!< Deepsleep operation in progress */
    DeepSleepStatus_Completed,        /*!< Deepsleep operation completed */
} DeepSleepStatus_t;

/*
 * Declare RPC API names and their arguments
 */
#ifdef ENABLE_DEEP_SLEEP

/**
 * @brief Initializes the underlying Deep Sleep Management module
 *
 * @return    Returns the status of the operation.
 * @retval    0 if successful, -1 if failure.
 */
int PLAT_DS_INIT(void);

/**
 * @brief Sets the CPE's Power State to Deep Sleep
 *
 * @param [in] deep_sleep_timeout   - The deep sleep wakeup timeout. Min value 0
 * @param [out] isGPIOWakeup        - Wakeup reason. TRUE for user related wake up, false for not
 *
 * @return  Returns the status of the operation.
 * @retval  0 if successful, -1 if failure.
 * 
 * @note    if deep_sleep_timeout is set 0, external intervention to wake up is required. @see DeepSleep_WakeupReason_t
 *
 * @pre     PLAT_DS_INIT() must be called before calling this API
 * @post    PLAT_DS_DeepSleepWakeup() must be called after calling this API
 *
 * @warning  This API is Not thread safe
 *
 */
int PLAT_DS_SetDeepSleep(uint32_t deep_sleep_timeout, bool *isGPIOWakeup);

/**
 * @brief Sets or configures the platform status after deepsleep wake-up
 * 
 * @pre      PLAT_DS_INIT() must be called before calling this API
 *
 * @note Calling this API before PLAT_DS_SetDeepSleep() should not result in any unexpected behavior
 *
 * @warning  This API is Not thread safe
 *
 */
void PLAT_DS_DeepSleepWakeup(void);

/**
 * @brief Terminates the CPE Deep Sleep Management module.
 *
 * All data structures used within Deep Sleep Management module must be reset and any
 * Deep Sleep Management specific handles and resources must be released
 */
void PLAT_DS_TERM(void);

/** Sets the timer for deep sleep ,timer is set explicitly by client of deep sleep manager, 
 * then the STB will accept the timer value, and go to sleep when sleep timer is expired.
 */
#define IARM_BUS_DEEPSLEEPMGR_API_SetDeepSleepTimer		"SetDeepSleepTimer" 
#define IARM_BUS_DEEPSLEEPMGR_API_GetLastWakeupReason		"GetLastWakeupReason"
#define IARM_BUS_DEEPSLEEPMGR_API_GetLastWakeupKeyCode		"GetLastWakeupKeycode"

/**
 * @brief Structure which holds the HAL timeout in seconds
 */
typedef struct _IARM_Bus_DeepSleepMgr_SetDeepSleepTimer_Param_t {
	unsigned int timeout;        /*!< Timeout for deep sleep in seconds*/ 
} IARM_Bus_DeepSleepMgr_SetDeepSleepTimer_Param_t;

/**
 * @brief Enum for all possible Wakeup Reasons from DeepSleep State
 */
typedef enum _DeepSleep_WakeupReason_t
{
   DEEPSLEEP_WAKEUPREASON_IR = 0,           /*!< Deepsleep Wakeup Reason is IR Remote event */
   DEEPSLEEP_WAKEUPREASON_RCU_BT,           /*!< Deepsleep Wakeup Reason is RCU Bluetooth Remote event */
   DEEPSLEEP_WAKEUPREASON_RCU_RF4CE,        /*!< Deepsleep Wakeup Reason is RCU RF4CE Remote event */
   DEEPSLEEP_WAKEUPREASON_GPIO,             /*!< Deepsleep Wakeup Reason is GPIO event */
   DEEPSLEEP_WAKEUPREASON_LAN,              /*!< Deepsleep Wakeup Reason is LAN event */
   DEEPSLEEP_WAKEUPREASON_WLAN,             /*!< Deepsleep Wakeup Reason is Wireless LAN event */
   DEEPSLEEP_WAKEUPREASON_TIMER,            /*!< Deepsleep Wakeup Reason is Clock Timer event */
   DEEPSLEEP_WAKEUPREASON_FRONT_PANEL,      /*!< Deepsleep Wakeup Reason is Front Panel Button event */
   DEEPSLEEP_WAKEUPREASON_WATCHDOG,         /*!< Deepsleep Wakeup Reason is Watchdog Timer event */
   DEEPSLEEP_WAKEUPREASON_SOFTWARE_RESET,   /*!< Deepsleep Wakeup Reason is S/W Reset */
   DEEPSLEEP_WAKEUPREASON_THERMAL_RESET,    /*!< Deepsleep Wakeup Reason is Thermal Reset event */
   DEEPSLEEP_WAKEUPREASON_WARM_RESET,       /*!< Deepsleep Wakeup Reason is Warm Temperature Reset event */
   DEEPSLEEP_WAKEUPREASON_COLDBOOT,         /*!< Deepsleep Wakeup Reason is Cold Boot event */
   DEEPSLEEP_WAKEUPREASON_STR_AUTH_FAILURE, /*!< Deepsleep Wakeup Reason is STR Authentication Failure event */
   DEEPSLEEP_WAKEUPREASON_CEC,              /*!< Deepsleep Wakeup Reason is HDMI CEC Failure event */
   DEEPSLEEP_WAKEUPREASON_PRESENCE,         /*!< Deepsleep Wakeup Reason is Motion Detection event */
   DEEPSLEEP_WAKEUPREASON_VOICE,            /*!< Deepsleep Wakeup Reason is User Voice event */
   DEEPSLEEP_WAKEUPREASON_UNKNOWN          /*!< Deepsleep Wakeup Reason is Unknown Reason event */
}DeepSleep_WakeupReason_t;

/**
 * @brief Struct for handling the wake up key code
 * 
 * The wake up key code is used to identify the wakeup reason.
 * 
 */
typedef struct _IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t {
	unsigned int keyCode;   /*!< Wake up keycode. */
} IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t;

/**
 * @brief Gets the CPE's Last wakeup reason
 *
 *
 * @param [out] wakeupReason    - Reason for CPE's last wake up
 *
 * @return    Returns the status of the operation.
 * @retval    0 if successful, -1 if failure.
 *
 * @pre PLAT_DS_INIT() must be called before calling this API
 *
 * @warning This API is Not thread safe
 *
 * @see PLAT_DS_GetLastWakeupKeyCode(), DeepSleep_WakeupReason_t
 *
 */
int  PLAT_DS_GetLastWakeupReason(DeepSleep_WakeupReason_t *wakeupReason);

/**
 * @brief Gets the CPE's Last wakeup key code
 *
 * @param [out] wakeupKeyCode   - The user wakeup code
 *
 * @return    Returns the status of the operation.
 * @retval    0 if successful, -1 if failure.
 *
 * @pre PLAT_DS_INIT() must be called before calling this API
 *
 * @warning This API is Not thread safe
 *
 * @note Caller is responsible for validating the returned keycode
 *
 * @see PLAT_DS_GetLastWakeupReason(), DeepSleepMgr_WakeupKeyCode_Param_t
 *
 */
int PLAT_DS_GetLastWakeupKeyCode(IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t *wakeupKeyCode);
#endif

#ifdef __cplusplus
}
#endif
#endif

/** @} */ // End of Deepsleep_Mgr_H
/** @} */ // End of Deepsleep_Manager_HAL
/** @} */ // End Deepsleep_Manager
/** @} */ // End of HPK
