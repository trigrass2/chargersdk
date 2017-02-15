/**
* @file taskcreate.h
* @brief
* @author rgw
* @version v1.0
* @date 2017-01-17
*/
#ifndef  __TASKCREATE_H
#define  __TASKCREATE_H

#include "includes.h"

#define defTIMERID_Temp             0
#define defTIMERID_LockState        1
#define defTIMERID_CPCCState        2
#define defTIMERID_ChargingData     3
#define defTIMERID_EVSEState        4

extern const char *TASKNAME_CLI;
extern const char *TASKNAME_GUI;
extern const char *TASKNAME_Touch;
extern const char *TASKNAME_OTA;
extern const char *TASKNAME_EVSERemote;
extern const char *TASKNAME_EVSERFID;
extern const char *TASKNAME_EVSECharge;
extern const char *TASKNAME_EVSEMonitor;
extern const char *TASKNAME_EVSEError;
extern const char *TASKNAME_EVSEData;

extern EventGroupHandle_t xHandleEventGroupRFID;

void SysTaskCreate (void);
void AppTaskCreate (void);
void AppObjCreate(void);

#endif