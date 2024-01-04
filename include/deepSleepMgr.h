/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
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
 * @defgroup IARMMGR_HAL HAL Types and Public API
 * Described herein are the IARM-Bus HAL types and functions that are part of the Power
 * Manager, Deep Sleep Manager and IR Manager applications.
 * @ingroup IARM_MGR
 *
 * @defgroup IARM_DEEPSLEEP_MGR_API Deep Sleep Manager (HAL Types and Public API)
 * Deep sleep is a power saving mode which turns off STB subsystems such as A/V, HDMI, front panels, HDD etc.
 * Provides API defines the structures and functions for the IARM-Bus Deep Sleep Manager interface.
 * - The main purpose is to bring down the power consumption to about 15% of the actual usage.
 * - It can also be triggered in other scenarios such as thermal shutdown in case of the temperature
 * is above threshold for certain period.
 * - When the STB goes in a normal power saving mode, it may be woken up for scheduled maintenance in
 * a predetermined time.
 * @ingroup IARMMGR_HAL
 */
/**
* @defgroup iarmmgrs
* @{
* @defgroup hal
* @{
**/
#ifndef _IARM_BUS_DEEPSLEEPMGR_H
#define _IARM_BUS_DEEPSLEEPMGR_H
#include "libIARM.h"
#include "libIBusDaemon.h"
/**
 * @addtogroup IARM_DEEPSLEEP_MGR_API
 * @{
 */
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
 * @brief This API Initializes the underlying Deep Sleep Management module.
 *
 * @return    Returns the status of the operation.
 * @retval    0 if successful, appropiate error code otherwise.
 */
int PLAT_DS_INIT(void);
/**
 * @brief This API is used to enter the CPE into Deep Sleep Mode.
 *
 * This function sets the CPE's Power State to Deep Sleep.
 *
 * @param [in] deep_sleep_timeout  The deep sleep wakeup timeout.
 *
 * @return  Return the status of the operation
 */
int PLAT_DS_SetDeepSleep(uint32_t deep_sleep_timeout, bool *isGPIOWakeup);
/**
 * @brief This function wakes up the CPE from deep sleep mode.
 */
void PLAT_DS_DeepSleepWakeup(void);
/**
 * @brief This function terminates the Deep Sleep manager.
 *
 * This function must terminate the CPE Deep Sleep Management module. It must reset any data
 * structures used within Deep Sleep Management module and release any Deep Sleep Management
 * specific handles and resources.
 */
void PLAT_DS_TERM(void);
/** Sets the timer for deep sleep ,timer is set explicitly by client of deep sleep manager, 
 * then the STB will accept the timer value, and go to sleep when sleep timer is expired.
 */
#define IARM_BUS_DEEPSLEEPMGR_API_SetDeepSleepTimer		"SetDeepSleepTimer" 
#define IARM_BUS_DEEPSLEEPMGR_API_GetLastWakeupReason		"GetLastWakeupReason"
#define IARM_BUS_DEEPSLEEPMGR_API_GetLastWakeupKeyCode		"GetLastWakeupKeycode"
/**
 * @brief Structure which holds the Deep sleep manager timeout.
 */
typedef struct _IARM_Bus_DeepSleepMgr_SetDeepSleepTimer_Param_t {
	unsigned int timeout;        /*!< Timeout for deep sleep in seconds*/ 
} IARM_Bus_DeepSleepMgr_SetDeepSleepTimer_Param_t;
typedef enum _DeepSleep_WakeupReason_t
{
   DEEPSLEEP_WAKEUPREASON_IR = 0,
   DEEPSLEEP_WAKEUPREASON_RCU_BT,
   DEEPSLEEP_WAKEUPREASON_RCU_RF4CE,
   DEEPSLEEP_WAKEUPREASON_GPIO,
   DEEPSLEEP_WAKEUPREASON_LAN,
   DEEPSLEEP_WAKEUPREASON_WLAN,
   DEEPSLEEP_WAKEUPREASON_TIMER,
   DEEPSLEEP_WAKEUPREASON_FRONT_PANEL,
   DEEPSLEEP_WAKEUPREASON_WATCHDOG,
   DEEPSLEEP_WAKEUPREASON_SOFTWARE_RESET,
   DEEPSLEEP_WAKEUPREASON_THERMAL_RESET,
   DEEPSLEEP_WAKEUPREASON_WARM_RESET,
   DEEPSLEEP_WAKEUPREASON_COLDBOOT,
   DEEPSLEEP_WAKEUPREASON_STR_AUTH_FAILURE,
   DEEPSLEEP_WAKEUPREASON_CEC,
   DEEPSLEEP_WAKEUPREASON_PRESENCE,
   DEEPSLEEP_WAKEUPREASON_VOICE,
   DEEPSLEEP_WAKEUPREASON_UNKNOWN
}DeepSleep_WakeupReason_t;
typedef struct _IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t {
	unsigned int keyCode;
} IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t;
int  PLAT_DS_GetLastWakeupReason(DeepSleep_WakeupReason_t *wakeupReason);
int PLAT_DS_GetLastWakeupKeyCode(IARM_Bus_DeepSleepMgr_WakeupKeyCode_Param_t *wakeupKeyCode);
/** @} */ //End of Doxygen tag
#endif
#ifdef __cplusplus
}
#endif
#endif
/** @} */
/** @} */
