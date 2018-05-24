/**
* @file connector.c
* @brief 定义最小充电单元需要的数据与操作
* @author rgw
* @version v1.0
* @date 2017-01-18
*/
#include <stdlib.h>
#include <string.h>
#include "evse_globals.h"
#include "connector.h"
#include "bsp.h"
#include "user_app.h"
#include "cJSON.h"
#include "stringName.h"
#include "cfg_parse.h"
#include "electric_energy_meter.h"
#include "timercallback.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "timers.h"

/**
 * @fn  static int SetSignalPool(void *pvDev, uint32_t block, uint32_t bit)
 *
 * @brief   Sets signal pool
 *
 * @param [in,out]  pvDev   If non-null, the pv development.
 * @param           block   The block.
 * @param           bit     The bit.
 *
 * @return  An int.
 */

static int SetSignalPool(void *pvDev, uint32_t block, uint32_t bit)
{
    CON_t *pCON;
    
    pCON = (CON_t *)pvDev;
    if (block >= CON_MAX_SIGNAL_BLOCK)
    {
        while (1)
            ;
    }
    pCON->status.ulSignalPool[block] |= bit;
    
    return 1;
}

/**
 * @fn  static int ClrSignalPool(void *pvDev, uint32_t block, uint32_t bit)
 *
 * @brief   Clear signal pool
 *
 * @param [in,out]  pvDev   If non-null, the pv development.
 * @param           block   The block.
 * @param           bit     The bit.
 *
 * @return  An int.
 */

static int ClrSignalPool(void *pvDev, uint32_t block, uint32_t bit)
{
    CON_t *pCON;
    
    pCON = (CON_t *)pvDev;
    if (block >= CON_MAX_SIGNAL_BLOCK)
    {
        while (1)
            ;
    }
    pCON->status.ulSignalPool[block] &= ~bit;
    
    return 1;
}

/**
 * @fn  static int GetSignalPool(void *pvDev, uint32_t block, uint32_t bit)
 *
 * @brief   Gets signal pool
 *
 * @param [in,out]  pvDev   If non-null, the pv development.
 * @param           block   The block.
 * @param           bit     The bit.
 *
 * @return  The signal pool.
 */

static int GetSignalPool(void *pvDev, uint32_t block, uint32_t bit)
{
    CON_t *pCON;
    
    pCON = (CON_t *)pvDev;
    if (block >= CON_MAX_SIGNAL_BLOCK)
    {
        while (1)
            ;
    }
    if ((pCON->status.ulSignalPool[block] & bit) == bit)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*---------------------------------------------------------------------------*/
/*                               设置充电接口信息到配置文件                    */
/*---------------------------------------------------------------------------*/

/**
 * @fn  static ErrorCode_t SetCONCfg(void *pvCON, char *jnItemString, void *pvCfgParam, uint8_t type)
 *
 * @brief   Sets con configuration
 *
 * @param [in,out]  pvCON           If non-null, the pv con.
 * @param [in,out]  jnItemString    If non-null, the jn item string.
 * @param [in,out]  pvCfgParam      If non-null, the pv configuration parameter.
 * @param           type            The type.
 *
 * @return  An ErrorCode_t.
 */

static ErrorCode_t SetCONCfg(void *pvCON, char *jnItemString, void *pvCfgParam, uint8_t type)
{
    cJSON *jsEVSECfgObj;
    cJSON *jsCONArray;
    cJSON *jsCONObj;
    cJSON *jsItem;
    ErrorCode_t errcode;
    CON_t *pCON;
    uint8_t ucCONID;


    errcode = ERR_NO;
    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    jsEVSECfgObj = GetCfgObj(pathEVSECfg, &errcode);
    if(jsEVSECfgObj == NULL)
    {
        return errcode;
    }
    jsCONArray = cJSON_GetObjectItem(jsEVSECfgObj, jnCONArray);
    jsCONObj = cJSON_GetArrayItem(jsCONArray, ucCONID);
    jsItem = jsCONObj->child;
    do
    {
        if(strcmp(jsItem->string, jnItemString) == 0)
        {
            switch(type)
            {
            case ParamTypeU8:
                cJSON_ReplaceItemInObject(jsCONObj, jnItemString, cJSON_CreateNumber(*((uint8_t *)pvCfgParam)));
                break;
            case ParamTypeU16:
                cJSON_ReplaceItemInObject(jsCONObj, jnItemString, cJSON_CreateNumber(*((uint16_t *)pvCfgParam)));
                break;
            case ParamTypeU32:
                cJSON_ReplaceItemInObject(jsCONObj, jnItemString, cJSON_CreateNumber(*((uint32_t *)pvCfgParam)));
                break;
            case ParamTypeDouble:
                cJSON_ReplaceItemInObject(jsCONObj, jnItemString, cJSON_CreateNumber(*((double *)pvCfgParam)));
                break;
            case ParamTypeString:
                cJSON_ReplaceItemInObject(jsCONObj, jnItemString, cJSON_CreateString((uint8_t *)pvCfgParam));
                break;
            case ParamTypeList:
                break;
            default:
                break;
            }
            break;//退出while循环
        }
        else
        {
            jsItem = jsItem->next;
        }
    }
    while(jsItem != NULL);
    errcode = SetCfgObj(pathEVSECfg, jsEVSECfgObj);

    return errcode;
}

/*---------------------------------------------------------------------------*/
/*                              从文件获取充电接口信息                         */
/*---------------------------------------------------------------------------*/

/**
 * @fn  static ErrorCode_t GetCONType(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets con type
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The con type.
 *
 * ### todo (rgw#1#): 增加枪充电类型CONType.
 */

static ErrorCode_t GetCONType(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint8_t tmpType;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpType = defCONType_AC;
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnType);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpType = jsItem->valueint;

    /*********************/
    if(tmpType > 0 && tmpType < 6)
    {
        pCON->info.ucCONType = tmpType;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }
    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetCONQRCode(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets conqr code
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The conqr code.
 */

static ErrorCode_t GetCONQRCode(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint8_t tmpQRCode[defQRCodeLength];
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    memset(tmpQRCode, 0, defQRCodeLength);
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnQRCode);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    strncpy(tmpQRCode, jsItem->valuestring, strlen(jsItem->valuestring));

    /*********************/
    if(strlen(tmpQRCode) > 0)
    {
        strncpy(pCON->info.strQRCode, tmpQRCode, strlen(tmpQRCode));
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }
    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetSocketType(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets socket type
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The socket type.
 */

static ErrorCode_t GetSocketType(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint8_t tmpType;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpType = defSocketTypeB;
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;
    /** @todo (rgw#1#): 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnSocketType);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpType = jsItem->valueint;

    /*********************/
    if(tmpType == defSocketTypeB || tmpType == defSocketTypeC)
    {
        pCON->info.ucSocketType = tmpType;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetVolatageLimits(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets volatage limits
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The volatage limits.
 */

static ErrorCode_t GetVolatageLimits(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpVoltUpLim;
    double tmpVoltLowLim;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpVoltUpLim = 250;//(V)
    tmpVoltLowLim = 190;//(V)
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;
    /** @todo (rgw#1#): 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnVolatageUpperLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpVoltUpLim = jsItem->valuedouble;
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnVolatageLowerLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpVoltLowLim = jsItem->valuedouble;

    /*********************/
    if(tmpVoltLowLim > 0 && tmpVoltUpLim > 0)
    {
        pCON->info.dVolatageUpperLimits = tmpVoltUpLim;
        pCON->info.dVolatageLowerLimits = tmpVoltLowLim;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetACTempLimits(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets ac temporary limits
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  a c temporary limits.
 */

static ErrorCode_t GetACTempLimits(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpACTempUpperLim;
    double tmpACTempLowerLim;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpACTempUpperLim = 105;//(℃)
    tmpACTempLowerLim = -40;//(℃)
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnACTempUpperLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpACTempUpperLim = jsItem->valuedouble;
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnACTempLowerLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpACTempLowerLim = jsItem->valuedouble;

    /*********************/
    if(tmpACTempUpperLim < 1000 && tmpACTempLowerLim > -100)
    {
        pCON->info.dACTempUpperLimits = tmpACTempUpperLim;
        pCON->info.dACTempLowerLimits = tmpACTempLowerLim;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetSocketTempLimits(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets socket temporary limits
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The socket temporary limits.
 */

static ErrorCode_t GetSocketTempLimits(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpSocketTempUpperLim;
    double tmpSocketTempLowerLim;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpSocketTempUpperLim = 105;//(℃)
    tmpSocketTempLowerLim = -40;//(℃)
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnSocketTempUpperLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpSocketTempUpperLim = jsItem->valuedouble;
    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnSocketTempLowerLimits);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpSocketTempLowerLim = jsItem->valuedouble;

    /*********************/
    if(tmpSocketTempUpperLim < 1000 && tmpSocketTempLowerLim > -100)
    {
        pCON->info.dSocketTempUpperLimits = tmpSocketTempUpperLim;
        pCON->info.dSocketTempLowerLimits = tmpSocketTempLowerLim;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetRatedCurrent(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets rated current
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The rated current.
 */

static ErrorCode_t GetRatedCurrent(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpCurr;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpCurr = 32;//(A)
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */

    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnRatedCurrent);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpCurr = jsItem->valuedouble;

    /*********************/
    if(tmpCurr > 0)
    {
        pCON->info.dRatedCurrent = tmpCurr;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetRatedPower(void *pvCON, void *pvCfgObj)
 *
 * @brief   Gets rated power
 *
 * @param [in,out]  pvCON       If non-null, the pv con.
 * @param [in,out]  pvCfgObj    If non-null, the pv configuration object.
 *
 * @return  The rated power.
 */

static ErrorCode_t GetRatedPower(void *pvCON, void *pvCfgObj)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpPow;
    ErrorCode_t errcode;

    cJSON *jsItem;
    cJSON *pCONCfgObj;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpPow = 7;//(kW)
    errcode = ERR_NO;

    pCONCfgObj = (cJSON *)pvCfgObj;

    /** 从文件获取 */

    jsItem = cJSON_GetObjectItem(pCONCfgObj, jnRatedPower);
    if(jsItem == NULL)
    {
        return ERR_FILE_PARSE;
    }
    tmpPow = jsItem->valuedouble;

    /*********************/
    if(tmpPow > 0)
    {
        pCON->info.dRatedPower = tmpPow;
    }
    else
    {
        errcode = ERR_FILE_PARAM;
    }

    return  errcode;
}

/**
 * @fn  static ErrorCode_t GetCONCfg(void *pvCON, void *pvCfgObj)
 *
 * @brief   从cfg文件获取充电枪配置
 *
 * @param [in,out]  pvCON       void*.
 * @param [in,out]  pvCfgObj    void* NULL.
 *
 * @return  ErrorCode_t.
 */

static ErrorCode_t GetCONCfg(void *pvCON, void *pvCfgObj)
{
    cJSON *jsCfgObj;
    cJSON *jsCONArray;
    cJSON *jsCONObj;
    int iArraySize;

    CON_t *pCON;
    int i;
    ErrorCode_t errcode = ERR_NO;

    pCON = (CON_t *)pvCON;

    /*json解析*/
    jsCfgObj = GetCfgObj(pathEVSECfg, &errcode);
    if(jsCfgObj == NULL)
    {
        //errcode 已经在GetCfgObj中获得
        goto exit;
    }
    /*取出CON相关配置*/
    jsCONArray = cJSON_GetObjectItem(jsCfgObj, jnCONArray);
    if(jsCONArray == NULL)
    {
        errcode = ERR_FILE_PARSE;
        goto exit_parse;
    }
    iArraySize = cJSON_GetArraySize(jsCONArray);//有多少个充电枪配置
    if(iArraySize != pEVSE->info.ucTotalCON)
    {
        errcode = ERR_FILE_PARAM;
        goto exit_parse;
    }
    jsCONObj = cJSON_GetArrayItem(jsCONArray, pCON->info.ucCONID);
    if(jsCONObj == NULL)
    {
        errcode = ERR_FILE_PARSE;
        goto exit_parse;
    }
    THROW_ERROR(defDevID_File, errcode = GetCONType(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetCONType()");
    THROW_ERROR(defDevID_File, errcode = GetSocketType(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetSocketType()");
    THROW_ERROR(defDevID_File, errcode = GetVolatageLimits(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetVolatageLimits()");
    THROW_ERROR(defDevID_File, errcode = GetACTempLimits(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetACTempLimits()");
    THROW_ERROR(defDevID_File, errcode = GetSocketTempLimits(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetSocketTempLimits()");
    THROW_ERROR(defDevID_File, errcode = GetRatedCurrent(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetRatedCurrent()");
    THROW_ERROR(defDevID_File, errcode = GetRatedPower(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetRatedPower()");
    THROW_ERROR(defDevID_File, errcode = GetCONQRCode(pvCON, jsCONObj), ERR_LEVEL_WARNING, "GetCONQRCode()");
exit_parse:
    cJSON_Delete(jsCfgObj);
exit:
    return errcode;
}

/*---------------------------------------------------------------------------*/
/*                              从驱动获取充电接口状态                         */
/*---------------------------------------------------------------------------*/


/** ！！！ 注意不同ID对硬件的不同操作 ！！！ */
/** ！！！ 注意不同ID对硬件的不同操作 ！！！ */
/** ！！！ 注意不同ID对硬件的不同操作 ！！！ */

/**
 * @fn  static ErrorCode_t GetChargingVoltage(void *pvCON)
 *
 * @brief   获取充电电压，检测精度 +/-0.1V
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_METER_FAULT.
 */

static ErrorCode_t GetChargingVoltage(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpVolt;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpVolt = 0;
    errcode = ERR_NO;

    /** 获取电压 */

    if(ucCONID == 0)
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpVolt = 220;
#else
        tmpVolt = Get_Electricity_meter_massage_voltage(ucCONID+1);//get_va();
#endif
    }
    if(ucCONID == 1)
    {
        //tmpVolt=get_va();
    }

    /*********************/

    pCON->status.dChargingVoltage = tmpVolt;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetChargingCurrent(void *pvCON)
 *
 * @brief   获取充电电流，检测精度+/-0.1A
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CON_METER_FAULT.
 */

static ErrorCode_t GetChargingCurrent(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpCurr;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpCurr = 0;
    errcode = ERR_NO;

    /** 获取电流 */
    if(Electricity_meter[ucCONID].flag.flag_erro == 1)
    {
        errcode = ERR_CON_METER_FAULT;
    }
    else
    {

#ifdef DEBUG_DIAG_DUMMY
        tmpCurr = 32;
#else
        tmpCurr = Get_Electricity_meter_massage_current(ucCONID + 1);
#endif

    }

    /*********************/

    pCON->status.dChargingCurrent = tmpCurr;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetChargingFrequence(void *pvCON)
 *
 * @brief   获取电源频率
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CON_METER_FAULT.
 */

static ErrorCode_t GetChargingFrequence(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpFreq;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
	tmpFreq = 0;
    errcode = ERR_NO;

    /** @todo (yuye#1#): 从电表获取 */
    //meter id 0 == CON id 0
#ifdef DEBUG_DIAG_DUMMY
    tmpFreq = 50;
#else
    if(Electricity_meter[ucCONID].flag.flag_erro == 1)
    {
        errcode = ERR_CON_METER_FAULT;
    }
    else
    {
        tmpFreq = Get_Electricity_meter_massage_frequency(ucCONID + 1);
    }

#endif

    /*********************/

    pCON->status.dChargingFrequence = tmpFreq;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetChargingPower(void *pvCON)
 *
 * @brief   Gets charging power
 *
 * @param [in,out]  pvCON   If non-null, the pv con.
 *
 * @return  The charging power.
 */

static ErrorCode_t GetChargingPower(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpPower;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
	tmpPower = 0;
    errcode = ERR_NO;

    /** @todo (yuye#1#): 从电表获取 */
    if(Electricity_meter[ucCONID].flag.flag_erro == 1)
    {
		tmpPower = pCON->status.dChargingPower;//通信失败后，获取上次的值
        errcode = ERR_CON_METER_FAULT;
    }
    else
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpPower = pCON->status.dChargingPower;
        tmpPower += 0.0001;
#else
        tmpPower = Get_Electricity_meter_massage_energy(ucCONID + 1);
#endif
    }

    /*********************/

    pCON->status.dChargingPower = tmpPower;

    return errcode;
}

/**
 * @fn  static ErrorCode_t SetCPSwitch(void *pvCON, uint8_t cmd)
 *
 * @brief   控制S1开关
 *
 * @param [in,out]  pvCON   void*.
 * @param           cmd     uint8_t   传递开关控制命令，SWITCH_ON /SWITCH_OFF.
 *
 * @return  ErrorCode_t.
 */

static ErrorCode_t SetCPSwitch(void *pvCON, uint8_t cmd)
{
    CON_t *pCON;
    uint8_t ucCONID;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    errcode = ERR_NO;

    /** ****************  */

    //...
    if (ucCONID == 0)
    {
        if (cmd == SWITCH_ON)
        {
            PWM1_535;
        }
        else if (cmd == SWITCH_OFF)
        {
            PWM1_1000;
        }
    }
    else if (ucCONID == 1)
    {


        if (cmd == SWITCH_ON)
        {
            PWM2_535;
        }
        else
        {
            PWM2_1000;
        }

    }


    /*********************/

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetCPState(void *pvCON)
 *
 * @brief   获取CP状态
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CP_FAULT.
 */

static ErrorCode_t GetCPState(void *pvCON)
{
    float cp1, cp2;
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpCPState;
    ErrorCode_t errcode;
    static uint32_t cp_err_cont;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpCPState = CP_12V;
    errcode = ERR_NO;

    /** ***********/

    if(ucCONID == 0)
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpCPState = CP_6V_PWM;
#else
        cp1 = Sys_samp.DC.CP1;//get_CP1();
        pCON->status.dCPVolt = cp1;
        if((cp1 < 12.8f) && (cp1 > 11.2f))
        {
            if(TIM2->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_12V_PWM;
                cp_err_cont = 0;
            }
            else
            {
                tmpCPState = CP_12V;
                cp_err_cont = 0;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else if((cp1 < 9.8f) && (cp1 > 8.2f))
        {
            if(TIM2->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_9V_PWM;
                cp_err_cont = 0;
            }
            else
            {
                tmpCPState = CP_9V;
                cp_err_cont = 0;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else if((cp1 < 6.8f) && (cp1 > 5.2f))
        {
            if(TIM2->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_6V_PWM;
                cp_err_cont = 0;
            }
            else
            {
                tmpCPState = CP_6V;
                cp_err_cont = 0;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else
        {
            printf_safe("CP_ERR %lf\n", cp1);
            printf_safe("CP_ERR %lf\n", cp1);
            printf_safe("CP_ERR %lf\n", cp1);
            printf_safe("CP_ERR %lf\n", cp1);
            printf_safe("CP_ERR %lf\n", cp1);
            printf_safe("CP_ERR %lf\n", cp1);
            tmpCPState = CP_ERR;
            if (tmpCPState == CP_ERR)
            {
                cp_err_cont++;
                tmpCPState = pCON->status.xCPState;
            }
            if (cp_err_cont >= 2)//50ms
            {
                SetCPSwitch(pCON, SWITCH_OFF);
                cp_err_cont = 0;
                tmpCPState = CP_ERR;
                pCON->status.ulSignalFault |= defSignalCON_Fault_CP;
                errcode =  ERR_CON_CP_FAULT;
            }
        }
#endif
    }
    else if(ucCONID == 1)
    {
        cp1 = get_CP2();
        if((cp2 < 12.8f) && (cp2 > 11.2f))
        {
            if(TIM4->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_12V_PWM;
            }
            else
            {
                tmpCPState = CP_12V;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else if((cp2 < 9.8f) && (cp2 > 8.2f))
        {
            if(TIM4->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_9V_PWM;
            }
            else
            {
                tmpCPState = CP_9V;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else if((cp2 < 6.8f) && (cp2 > 5.2f))
        {
            if(TIM2->CCR1 != TIMER_MAX)
            {
                tmpCPState = CP_6V_PWM;
            }
            else
            {
                tmpCPState = CP_6V;
            }
            pCON->status.ulSignalFault &= ~defSignalCON_Fault_CP;
        }
        else
        {
            tmpCPState = CP_ERR;
            pCON->status.ulSignalFault |= defSignalCON_Fault_CP;
            errcode =  ERR_CON_CP_FAULT;
        } ;
    }
    /*********************/

    pCON->status.xCPState = tmpCPState;

    return errcode;
}

/**
 * @fn  static ErrorCode_t SetLoadPercent(void *pvCON, uint8_t ucLoadPercent)
 *
 * @brief   设置PWM占空比 详情请看18487.1-2015 P22
 *
 * @param [in,out]  pvCON           void*.
 * @param           ucLoadPercent   uint8_t 负载百分比 0 ~ 100.
 *
 * @return  ErrorCode_t.
 */

static ErrorCode_t SetLoadPercent(void *pvCON, uint8_t ucLoadPercent)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint8_t tmpCPPWM;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpCPPWM = 53; //负载100%时，PWM=53, 负载50%时，PWM= 27
    errcode = ERR_NO;

    if (ucLoadPercent == pCON->status.ucLoadPercent)//相同负载百分比不进行设置
    {
        return errcode;
    }
    else//由于目前设置负载百分比会导致cp检测不准，暂时屏蔽该功能，技术突破后，屏蔽掉该else
    {
        pCON->status.ucLoadPercent = ucLoadPercent;
        return errcode;
    }
    /** ************* */
    if(ucCONID == 0)
    {
        TIM2->CCR1 = 1001 - (uint32_t)(5.3 * ucLoadPercent); //1001 - (53 * 10) * (ucLoadPercent / 100.0)
    }
    else if(ucCONID == 1)
    {
        TIM4->CCR1 = 1001 - (uint32_t)(5.3 * ucLoadPercent);
    }
    //负载百分比输入范围0~100；
    //PWM
    /*********************/

    pCON->status.ucLoadPercent = ucLoadPercent;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetCCState(void *pvCON)
 *
 * @brief   Gets Cc state
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CC_FAULT.
 */

static ErrorCode_t GetCCState(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpCCState;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpCCState = CC_NO;
    errcode = ERR_NO;

    /** *****************  */

#ifdef DEBUG_DIAG_DUMMY
    tmpCCState = CC_PE;
#else
    if(ucCONID == 0)
    {
        if(GET_CC1 == 0) //已经连接CC1点，PE连接正常
        {
            tmpCCState = CC_PE; 
            pCON->status.ulSignalState |= defSignalCON_State_CC;
        }
        else if(GET_CC1 == 1)
        {
            tmpCCState = CC_NO;
            pCON->status.ulSignalState &= ~defSignalCON_State_CC;
        }

    }
    else if(ucCONID == 1)
    {
        if(GET_CC2 == 0) //已经连接CC1点，PE连接正常
        {
            tmpCCState = CC_PE;
            pCON->status.ulSignalState |= defSignalCON_State_CC;
        }
        else if(GET_CC2 == 1)
        {
            tmpCCState = CC_NO;
            pCON->status.ulSignalState &= ~defSignalCON_State_CC;
        }
    }
#endif
    /*********************/

    pCON->status.xCCState = tmpCCState;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetPlugState(void *pvCON)
 *
 * @brief   获取插枪状态，应同时检测检测点1（CC）和检测点4（CP）
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CON_PLUG_FAULT.
 */

static ErrorCode_t GetPlugState(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint32_t tmpPlugState; //    PLUG,  UNPLUG,
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpPlugState = UNPLUG;
    errcode = ERR_NO;

    /** 检测插枪状态驱动接口  */
    if(pCON->info.ucSocketType == defSocketTypeB)
    {
        THROW_ERROR(ucCONID, GetCCState(pvCON), ERR_LEVEL_CRITICAL, "GetPlug->GetCC");
        THROW_ERROR(ucCONID, GetCPState(pvCON), ERR_LEVEL_CRITICAL, "GetPlug->GetCP");
        if(pCON->status.xCCState == CC_PE &&
                pCON->status.xCPState != CP_12V &&
                pCON->status.xCPState != CP_12V_PWM &&
                pCON->status.xCPState != CP_ERR)
        {
            tmpPlugState = PLUG;
        }
        else
        {
            tmpPlugState = UNPLUG;
        }
    }
    else if(pCON->info.ucSocketType == defSocketTypeC)
    {
        THROW_ERROR(ucCONID, GetCPState(pvCON), ERR_LEVEL_CRITICAL, "GetPlug->GetCP");
        if(pCON->status.xCPState != CP_12V &&
                pCON->status.xCPState != CP_12V_PWM &&
                pCON->status.xCPState != CP_ERR)
        {
            tmpPlugState = PLUG;
        }
        else
        {
            tmpPlugState = UNPLUG;
        }
    }
    /*********************/

    pCON->status.xPlugState = tmpPlugState;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetBTypeSocketLock(void *pvCON)
 *
 * @brief   B型连接枪锁状态
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_B_LOCK_FAULT.
 */

static ErrorCode_t GetBTypeSocketLock(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpLockState; //lock unlock
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpLockState = UNLOCK;
    errcode = ERR_NO;

    /** 实现代码  */

    if(ucCONID == 0)
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpLockState = LOCK;
#else
        if(GET_GUN_STATE_1 == 1)
        {
            tmpLockState = UNLOCK;
        }
        else if(GET_GUN_STATE_1 == 0)
        {
            tmpLockState = LOCK;
        }
#endif
    }
    else if(ucCONID == 1)
    {
        if(GET_GUN_STATE_2 == 1)
        {
            tmpLockState = UNLOCK;
        }
        else if(GET_GUN_STATE_2 == 0)
        {
            tmpLockState = LOCK;
        }
    }
    /*********************/


    pCON->status.xBTypeSocketLockState = tmpLockState;

    return errcode;
}

/**
 * @fn  static ErrorCode_t SetBTypeSocketLock(void *pvCON, uint8_t cmd)
 *
 * @brief   B型连接枪锁开关
 *
 * @param [in,out]  pvCON   void*.
 * @param           cmd     uint8_t   开关控制，SWITCH_ON /SWITCH_OFF.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_CANT_LOCK.
 */

static ErrorCode_t SetBTypeSocketLock(void *pvCON, uint8_t cmd)
{
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpLockState;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpLockState = UNLOCK;
    errcode = ERR_NO;

    /**  执行锁止动作 */
    if(ucCONID == 0)
    {
        if(cmd == SWITCH_ON)
        {
            Close_gun_1();
        }
        else
        {
            Open_gun_1();
        }
    }
    else if(ucCONID == 1)
    {
        if(cmd == SWITCH_ON)
        {
            Close_gun_2();
        }
        else
        {
            Open_gun_2();
        }
    }
    errcode = GetBTypeSocketLock(pvCON);

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetACLTemp(void *pvCON)
 *
 * @brief   获取L进线温度
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_ACLTEMP_DECT_FAULT.
 */

static ErrorCode_t GetACLTemp(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double  tmpACLTemp;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpACLTemp = 0;
    errcode = ERR_NO;

    /** 实现代码  */
    if(ucCONID == 0)
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpACLTemp = 40;
#else
	    
//        tmpACLTemp = (double)Sys_samp.DC.TEMP1;
        tmpACLTemp = (double)get_dc_massage(TEMP_L_IN);
        if(tmpACLTemp > 200 || tmpACLTemp < -40)
        {
            errcode = ERR_CON_ACTEMP_DECT_FAULT;
        }
#endif
    }
    else if(ucCONID == 1)
    {
    }


    /*********************/

    pCON->status.dACLTemp = tmpACLTemp;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetACNTemp(void *pvCON)
 *
 * @brief   Gets a cn temporary
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_ACNTEMP_DECT_FAULT.
 */

static ErrorCode_t GetACNTemp(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpACNTemp;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpACNTemp = 0;
    errcode = ERR_NO;

    /** 实现代码  */

    //...
    if(ucCONID == 0)
    {
#ifdef DEBUG_DIAG_DUMMY
        tmpACNTemp = 25;
#else
        
        //tmpACNTemp = (double)Sys_samp.DC.TEMP3;
        tmpACNTemp = (double)get_dc_massage(TEMP_L_OUT); 
        if(tmpACNTemp > 200 || tmpACNTemp < -40)
        {
           errcode = ERR_CON_ACTEMP_DECT_FAULT;
        }
#endif
    }
    else if(ucCONID == 1)
    {
    }

    /*********************/

    pCON->status.dACNTemp = tmpACNTemp;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetBTypeSocketTemp1(void *pvCON)
 *
 * @brief   Gets b type socket temporary 1
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_BTEMP1_DECT_FAULT.
 */

static ErrorCode_t GetBTypeSocketTemp1(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpTemp;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpTemp = 0;
    errcode = ERR_NO;

    /** 实现代码  */

    if(ucCONID == 0)
    {
#ifndef DEBUG_DIAG_DUMMY_C
        tmpTemp = 25;
#else
        tmpTemp = get_dc_massage(TEMP_GUN1_NEGATIVE);
        if(tmpTemp > 200 || tmpTemp < -40)
        {
            errcode = ERR_CON_BTEMP1_DECT_FAULT;
        }
#endif
    }
    else if(ucCONID == 1)
    {
        tmpTemp = get_dc_massage(TEMP_GUN2_NEGATIVE);
        if(tmpTemp > 100 || tmpTemp < -40)
        {
            errcode = ERR_CON_BTEMP1_DECT_FAULT;
        }
    }
    /*********************/

    pCON->status.dBTypeSocketTemp1 = tmpTemp;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetBTypeSocketTemp2(void *pvCON)
 *
 * @brief   Gets b type socket temporary 2
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_BTEMP2_DECT_FAULT.
 */

static ErrorCode_t GetBTypeSocketTemp2(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    double tmpTemp;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpTemp = 0;
    errcode = ERR_NO;

    /** 实现代码  */

    //...
    if(ucCONID == 0)
    {
#ifndef DEBUG_DIAG_DUMMY_C
        tmpTemp = 25;
#else
        tmpTemp = get_dc_massage(TEMP_GUN1_POSITIVE);
        if(tmpTemp > 100 || tmpTemp < -40)
        {
            errcode = ERR_CON_BTEMP2_DECT_FAULT;
        }
#endif
    }
    else if(ucCONID == 1)
    {
        tmpTemp = get_dc_massage(TEMP_GUN2_POSITIVE);
        if(tmpTemp > 100 || tmpTemp < -40)
        {
            errcode = ERR_CON_BTEMP2_DECT_FAULT;
        }
    }

    /*********************/

    pCON->status.dBTypeSocketTemp2 = tmpTemp;

    return errcode;
}

/**
 * @fn  static ErrorCode_t GetRelayState(void *pvCON)
 *
 * @brief   获取输出继电器状态
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t.
 */

static ErrorCode_t GetRelayState(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    uint8_t tmpLStat;
    uint8_t tmpNStat;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    tmpLStat = SWITCH_OFF;
    tmpNStat = SWITCH_OFF;
    errcode = ERR_NO;

    /** 实现代码  */
#ifdef DEBUG_DIAG_DUMMY
    tmpLStat = SWITCH_ON;
    tmpNStat = tmpLStat;
#else

    tmpLStat = Get_State_relay();//1 : switch on
    tmpNStat = tmpLStat;
    if (tmpLStat == SWITCH_ON)
    {
        pCON->status.ulSignalState |= defSignalCON_State_AC_A_Relay;
        pCON->status.ulSignalState |= defSignalCON_State_AC_N_Relay;
    }
    else if(tmpLStat == SWITCH_OFF)
    {
        pCON->status.ulSignalState &= ~defSignalCON_State_AC_A_Relay;
        pCON->status.ulSignalState &= ~defSignalCON_State_AC_N_Relay;
    }
#endif
    /*********************/
    /* @todo (yuye#1#): 触电粘连处理2017年4月10日 */
//    if(触电粘连)
//    {
//        errcode = ERR_RELAY_PASTE;
//    }


    pCON->status.ucRelayLState = tmpLStat;
    pCON->status.ucRelayNState = tmpNStat;

    return errcode;
}

/**
 * @fn  static ErrorCode_t SetRelay(void *pvCON, uint8_t cmd)
 *
 * @brief   Sets a relay
 *
 * @param [in,out]  pvCON   void*.
 * @param           cmd     uint8_t SWITCH_ON SWITCH_OFF.
 *
 * @return  ErrorCode_t.
 */

static ErrorCode_t SetRelay(void *pvCON, uint8_t cmd)
{
    CON_t *pCON;
    uint8_t ucCONID;

    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    errcode = ERR_NO;


    /** 操作K1,K2输出继电器 */
    if(ucCONID == 0)
    {
        if(cmd == SWITCH_OFF)
        {
#ifdef DEBUG_DIAG_DUMMY
#else
            POWER_L_OPEN();
            POWER_N_OPEN();
#endif
        }
        else if(cmd == SWITCH_ON)
        {
#ifdef DEBUG_DIAG_DUMMY
#else
            POWER_L_CLOSE();
            POWER_N_CLOSE();
#endif
        }
    }
    else if(ucCONID == 1)
    {
        ;
    }
    /*********************/

    errcode = GetRelayState(pvCON);
    return errcode;
}

/**
 * @fn  static ErrorCode_t StartCharge(void *pvCON)
 *
 * @brief   Starts a charge
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_STARTCHARGE.
 */

static ErrorCode_t StartCharge(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpChargeStatus;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    errcode = ERR_NO;
    /**  操作输出继电器，保存继电器状态 */

    errcode = SetRelay(pvCON, SWITCH_ON);
    vTaskDelay(defRelayDelay);
    THROW_ERROR(ucCONID, errcode = pCON->status.GetRelayState(pCON), ERR_LEVEL_CRITICAL, "STATE_CON_STARTCHARGE");
    if (pCON->status.ucRelayLState == SWITCH_ON &&
            pCON->status.ucRelayNState == SWITCH_ON)
    {
        errcode = ERR_NO;
    }

    /*********************/
    return errcode;
}

/**
 * @fn  static ErrorCode_t StopCharge(void *pvCON)
 *
 * @brief   Stops a charge
 *
 * @param [in,out]  pvCON   void*.
 *
 * @return  ErrorCode_t
 *           ERR_NO ERR_STOPCHARGE.
 */

static ErrorCode_t StopCharge(void *pvCON)
{
    CON_t *pCON;
    uint8_t ucCONID;
    CONStatusType_t tmpChargeStatus;
    EventBits_t uxBits;
    ErrorCode_t errcode;

    pCON = (CON_t *)pvCON;
    ucCONID = pCON->info.ucCONID;
    errcode = ERR_NO;

    SetCPSwitch(pCON, SWITCH_OFF);

#ifdef DEBUG_DIAG_DUMMY
    pCON->status.xCPState = CP_12V;
#endif

    if (pCON->status.xCPState == CP_6V ||
       pCON->status.xCPState == CP_9V ||
       pCON->status.xCPState == CP_12V)
    {
        uxBits = xEventGroupWaitBits(pCON->status.xHandleEventCharge,
            defEventBitCONS2Opened,
            pdFALSE,
            pdTRUE,
            3000);//S1转换到12V后S2应在100ms内断开，否则强制带载断电。
        //此处判断uxbits无意义，因为无论如何100ms内或者100ms外都要断电。
        if ((uxBits & defEventBitCONS2Opened) == defEventBitCONS2Opened)
        {
            printf_safe("S2 Opened OK\n");
        }
        else
        {
            printf_safe("S2 Opend timeout\n");
        }
    }
    else if (pCON->status.xCPState != CP_ERR) //PWM
    {
        //CP未转换;
        return ERR_CON_CP_SWITCH_FAULT;
    }
    else//CP_ERR
    {
        
    }
    errcode = SetRelay(pvCON, SWITCH_OFF);
    THROW_ERROR(ucCONID, errcode = GetRelayState(pCON), ERR_LEVEL_CRITICAL, "conAPI stop charge");
#ifdef DEBUG_DIAG_DUMMY
    pCON->status.ucRelayLState = SWITCH_OFF;
    pCON->status.ucRelayNState = SWITCH_OFF;
#endif
    if (pCON->status.ucRelayLState == SWITCH_OFF &&
        pCON->status.ucRelayNState == SWITCH_OFF)
    {
        errcode = ERR_NO;
    }
    else
    {
        errcode = ERR_RELAY_PASTE;
    }
    /*********************/
    return errcode;
}

/**
 * @fn  CON_t *CONGetHandle(uint8_t ucCONID)
 *
 * @brief   Con get handle
 *
 * @param   ucCONID The conid.
 *
 * @return  Null if it fails, else a pointer to a CON_t.
 */

CON_t *CONGetHandle(uint8_t ucCONID)
{
    CON_t *pCON;
    uint32_t ulTotalCON;

    ulTotalCON = pListCON->Total;

    if(ucCONID < ulTotalCON)
    {
        return (CON_t *)(pListCON->pListPointArray[ucCONID]);
    }
    else
    {
        return NULL;
    }
}

/**
 * @fn  CONState_t CONGetState(uint8_t ucCONID)
 *
 * @brief   Con get state
 *
 * @param   ucCONID The conid.
 *
 * @return  A CONState_t.
 */

CONState_t CONGetState(uint8_t ucCONID)
{
    CON_t *pCON = NULL;

    pCON = CONGetHandle(ucCONID);

    return pCON->state;
}

/**
 * @fn  static void CONDelete(CON_t *pCON)
 *
 * @brief   Con delete
 *
 * @param [in,out]  pCON    If non-null, the con.
 */

static void CONDelete(CON_t *pCON)
{
    vEventGroupDelete(pCON->status.xHandleEventCharge);
    vEventGroupDelete(pCON->status.xHandleEventOrder);
    vEventGroupDelete(pCON->status.xHandleEventException);
    vEventGroupDelete(pCON->status.xHandleEventTimerCBNotify);
    xTimerDelete(pCON->status.xHandleTimerRTData, 100);
    xTimerDelete(pCON->OrderTmp.xHandleTimerOrderTmp, 100);
    free(pCON);
    pCON = NULL;
}

/**
 * @fn  CON_t *CONCreate(uint8_t ucCONID )
 *
 * @brief   Con create
 *
 * @param   ucCONID The conid.
 *
 * @return  Null if it fails, else a pointer to a CON_t.
 */

CON_t *CONCreate(uint8_t ucCONID )
{
    CON_t *pCON;
    pCON = (CON_t *)malloc(sizeof(CON_t));
    if(pCON == NULL)
    {
        return NULL;
    }
    pCON->info.ucCONID                = ucCONID;
    pCON->info.ucCONType              = defCONType_AC;
    pCON->info.ucSocketType           = defSocketTypeC;
    pCON->info.dVolatageUpperLimits   = 0;
    pCON->info.dVolatageLowerLimits   = 0;
    pCON->info.dACTempUpperLimits     = 0;
    pCON->info.dACTempLowerLimits     = 0;
    pCON->info.dSocketTempUpperLimits = 0;
    pCON->info.dSocketTempLowerLimits = 0;
    pCON->info.dRatedCurrent          = 32;
    pCON->info.dRatedPower            = 7;
    memset(pCON->info.strQRCode, 0, sizeof(pCON->info.strQRCode));

    pCON->info.GetCONCfg = GetCONCfg;
    pCON->info.SetCONCfg = SetCONCfg;

    pCON->status.dCPVolt               = 0;
    pCON->status.dACLTemp              = 0;
    pCON->status.dACNTemp              = 0;
    pCON->status.dBTypeSocketTemp1     = 0;
    pCON->status.dBTypeSocketTemp2     = 0;
    pCON->status.dChargingCurrent      = 0;
    pCON->status.dChargingFrequence    = 0;
    pCON->status.dChargingVoltage      = 0;
    pCON->status.dChargingPower        = 0;
    pCON->status.xBTypeSocketLockState = 0;
    pCON->status.xCCState              = 0;
    pCON->status.xCPState              = 0;
    pCON->status.ucLoadPercent         = 100;//(%)
    pCON->status.xPlugState            = 0;
    pCON->status.xHandleEventCharge    = xEventGroupCreate();
    pCON->status.xHandleEventOrder     = xEventGroupCreate();
    pCON->status.xHandleEventException = xEventGroupCreate();
    pCON->status.xHandleEventTimerCBNotify = xEventGroupCreate();
    pCON->status.xHandleTimerVolt      = NULL;
    pCON->status.xHandleTimerCurr      = NULL;
    pCON->status.xHandleTimerFreq      = NULL;
    pCON->status.xHandleTimerCharge    = NULL;
    pCON->status.xHandleTimerRTData    = NULL;
    pCON->status.GetChargingVoltage    = GetChargingVoltage;
    pCON->status.GetChargingCurrent    = GetChargingCurrent;
    pCON->status.GetChargingFrequence  = GetChargingFrequence;
    pCON->status.GetChargingPower      = GetChargingPower;
    pCON->status.xVoltStat             = STATE_VOLT_OK;
    pCON->status.xCurrStat             = STATE_CURR_INIT;
    pCON->status.xFreqStat             = STATE_FREQ_OK;
    pCON->status.ulSignalState         = 0;
    pCON->status.ulSignalAlarm         = 0;
    pCON->status.ulSignalFault         = 0;

    pCON->status.GetCPState          = GetCPState;
    pCON->status.SetCPSwitch         = SetCPSwitch;
    pCON->status.SetLoadPercent      = SetLoadPercent;
    pCON->status.GetCCState          = GetCCState;
    pCON->status.GetPlugState        = GetPlugState;
    pCON->status.GetBTypeSocketLock  = GetBTypeSocketLock;
    pCON->status.SetBTypeSocketLock  = SetBTypeSocketLock;
    pCON->status.GetACLTemp          = GetACLTemp;
    pCON->status.GetACNTemp          = GetACNTemp;
    pCON->status.GetBTypeSocketTemp1 = GetBTypeSocketTemp1;
    pCON->status.GetBTypeSocketTemp2 = GetBTypeSocketTemp2;
    pCON->status.StartCharge         = StartCharge;
    pCON->status.StopCharge          = StopCharge;
    pCON->status.GetRelayState       = GetRelayState;
    pCON->status.SetRelay            = SetRelay;

    pCON->state = STATE_CON_IDLE;

    pCON->status.xHandleTimerRTData = xTimerCreate("TimerRemoteRTData",
                                      defRemoteRTDataCyc,
                                      pdTRUE,
                                      (void *)(int)ucCONID,
                                      vRemoteRTDataTimerCB);
    //order init
    OrderInit(&(pCON->order));
    //OrderTmp init
    OrderInit(&(pCON->OrderTmp.order));
    pCON->OrderTmp.ucCheckOrderTmp = 0;
    sprintf(pCON->OrderTmp.strOrderTmpPath, "%sOrderCON%d.tmp", pathSystemDir, pCON->info.ucCONID);
    pCON->OrderTmp.xHandleTimerOrderTmp = xTimerCreate("TimerOrderTmp",
                                                    defOrderTmpCyc,
                                                    pdTRUE,
                                                    (void *)(int)(pCON->info.ucCONID),
                                                    vOrderTmpTimerCB);

    return pCON;
}
