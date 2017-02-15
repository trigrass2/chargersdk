/**
* @file chargepoint.h
* @brief ������С��絥Ԫ��Ҫ�����������
* @author rgw
* @version v1.0
* @date 2017-01-18
*/
#ifndef  __CHARGEPOINT_H
#define  __CHARGEPOINT_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define defConnectorTypeB   0x0B
#define defConnectorTypeC   0x0C

typedef enum __ChargePointStateType
{
    NO_ERR = 0,
    //CP���
    CP_ERR,
    CP_12V,
    CP_9V,
    CP_9V_PWM,
    CP_6V_PWM,
    //CC���
    CC_PE,
    CC_NO,
    //��ǹ���
    PLUG,
    UNPLUG,
    //ǹ��
    LOCK,
    UNLOCK
}ChargePointStateType;

typedef struct _ChargePointInfo
{
    uint8_t ucChargePointID;           // ǹ��
    uint8_t ucConnectorType;           //0x0B 0x0C
    uint32_t ulVolatageUpperLimits;
    uint32_t ulVolatageLowerLimits;
    uint32_t ulCurrent;
    uint32_t ulPower;
}ChargePointInfo_t;

typedef struct _ChargePointState
{
    ChargePointStateType ulCPState;     // ����1 CP state --12V / 9V / 9V_PWM / 6V_PWM
    ChargePointStateType ulCCState;     // ����4 CC state --PE
    ChargePointStateType ulPlugState;
    double dACLTemp;
    double dACNTemp;
    double dBTypeConnectorTemp1;
    double dBTypeConnectorTemp2;
    ChargePointStateType BTypeConnectorLockState; //lock unlock
    double dChargingVoltage;
    double dChargingCurrent;
    double dChargingFrequence;
    EventGroupHandle_t xHandleEventGroupStartCharge;
    EventGroupHandle_t xHandleEventGroupStopCharge;
}ChargePointState_t;

typedef struct _ChargePoint
{
    ChargePointInfo_t info;
    ChargePointState_t state;

} ChargePoint_t;

double GetChargingVoltage(ChargePoint_t *pPoint);
double GetChargingCurrent(ChargePoint_t *pPoint);
double GetChargingFrequence(ChargePoint_t *pPoint);

ChargePointStateType GetCPState(ChargePoint_t *pPoint);
ChargePointStateType GetCCState(ChargePoint_t *pPoint);
uint32_t GetPlugState(ChargePoint_t *pPoint);
ChargePointStateType GetBTypeConnectorLock(ChargePoint_t *pPoint); //lock unlock
ChargePointStateType SetBTypeConnectorLock(ChargePoint_t *pPoint);
double GetACLTemp(ChargePoint_t *pPoint);
double GetACNTemp(ChargePoint_t *pPoint);
double GetBTypeConnectorTemp1(ChargePoint_t *pPoint);
double GetBTypeConnectorTemp2(ChargePoint_t *pPoint);
ChargePointStateType StartCharge(ChargePoint_t *pPoint);
ChargePointStateType StopCharge(ChargePoint_t *pPoint);

ChargePoint_t *ChargePointCreate(uint8_t ucChargePointID );
#endif