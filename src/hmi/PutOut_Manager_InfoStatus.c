﻿/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.30                          *
*        Compiled Jul  1 2015, 10:50:32                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "touchtimer.h"
// USER END

#include "DIALOG.h"

#include "includes.h"

#include "order.h"
#include "interface.h"
#include "utils.h"
#include "cJSON.h"
#include "evse_config.h"
#include "cfg_parse.h"
#include "stringName.h"
#include "errorcode.h"
#include <string.h>

#define BYTES_LEN 1024

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
// USER START (Optionally insert additional defines)
#define ID_IMAGE_0    (GUI_ID_USER + 0x1C)
#define ID_TEXT_1     (GUI_ID_USER + 0x0B)
#define ID_TEXT_2     (GUI_ID_USER + 0x0C)
#define ID_TEXT_3     (GUI_ID_USER + 0x0D)
#define ID_TEXT_4     (GUI_ID_USER + 0x0E)

#define ID_BUTTON_0  (GUI_ID_USER + 0x00)//信息
#define ID_BUTTON_1  (GUI_ID_USER + 0x01)//历史
#define ID_BUTTON_2  (GUI_ID_USER + 0x02)//系统
#define ID_BUTTON_3  (GUI_ID_USER + 0x03)//退出
#define ID_BUTTON_4  (GUI_ID_USER + 0x04)//模拟
#define ID_BUTTON_5  (GUI_ID_USER + 0x05)//状态
#define ID_TEXT_5  (GUI_ID_USER + 0x06)//
#define ID_EDIT_0  (GUI_ID_USER + 0x07)//
#define ID_TEXT_6  (GUI_ID_USER + 0x08)//

#define ID_LISTWHEEL_0  (GUI_ID_USER + 0x10)
#define ID_LISTWHEEL_1  (GUI_ID_USER + 0x11)
#define ID_LISTWHEEL_2  (GUI_ID_USER + 0x12)
#define ID_LISTWHEEL_3  (GUI_ID_USER + 0x13)
#define ID_LISTWHEEL_4  (GUI_ID_USER + 0x14)
#define ID_LISTWHEEL_5  (GUI_ID_USER + 0x15)
#define ID_MULTIEDIT_0  (GUI_ID_USER + 0x16)
#define ID_LISTVIEW_0   (GUI_ID_USER + 0x17)

#define ID_TimerTime    1
#define ID_TimerFlush   2
#define ID_TimerSignal  3

#define ALARM_COLUMNS   4
#define CHARGE_COLUMNS  20
#define DB_DEBUG    0

#define stateName "状态项"
#define stateValue "状态"
#define stateWork "工作状态"
#define stateScram "急停状态"
#define stateMeter "电表状态"
#define stateSocket1Temp "插座温度1"
#define stateSocket2Temp "插座温度2"
#define stateCurrent "输出电流"
#define stateRFID "RFID状态"
#define stateSocketLock "枪锁"
#define stateNTemp "零线温度"
#define stateACVol "交流电压"
#define stateACFreq "交流频率"
#define stateACFL "防雷器状态"
#define stateRelay "输出继电器"
#define stateCP "控制导引"

static WM_HTIMER _timerRTC,_timerData,_timerSignal;
uint16_t column_num,row_num;
static WM_HWIN _hWinManagerInfoStatus;
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { WINDOW_CreateIndirect, "Framewin", ID_WINDOW_0, 0, 20, 800, 300, 0, 0x64, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 20, 800, 276, 0, 0x0, 0 },//560,276
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/** @brief 状态内容编辑分析
 *
 * @param
 * @param
 * @return
 *
 */
static void Status_Content_Analy(WM_MESSAGE *pMsg)
{
	CON_t	*pcont;
	pcont = CONGetHandle(0);
    WM_HWIN hItem;

    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    /**< 工作状态 */
	if ((pcont->status.ulSignalState & defSignalCON_State_Working) == defSignalCON_State_Working)
	{
		LISTVIEW_SetItemText(hItem, 1, 0, "充电中");
	}
	else if ((pcont->status.ulSignalState & defSignalCON_State_Standby) == defSignalCON_State_Standby)
	{
		LISTVIEW_SetItemText(hItem, 1, 0, "空闲");
	}
	else if((pcont->status.ulSignalState & defSignalCON_State_Fault) == defSignalCON_State_Fault)
	{
		LISTVIEW_SetItemText(hItem, 1, 0, "故障");
	}

	/**< 急停状态 */
	if ((pEVSE->status.ulSignalAlarm & defSignalEVSE_Alarm_Scram) == defSignalEVSE_Alarm_Scram)
	{
		LISTVIEW_SetItemText(hItem, 3, 0, "按下");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 3, 0, "正常");
	}

	/**< 电表通信 */
	if ((pcont->status.ulSignalFault & defSignalCON_Fault_Meter) == defSignalCON_Fault_Meter)
	{
		LISTVIEW_SetItemText(hItem, 5, 0, "故障");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 5, 0, "正常");
	}

    /**< A插座温度1 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_SocketTemp1_Cri) == defSignalCON_Alarm_SocketTemp1_Cri)
	{
		LISTVIEW_SetItemText(hItem, 1, 1, "过温");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 1, 1, "正常");
	}

    /**< A插座温度2 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_SocketTemp2_Cri) == defSignalCON_Alarm_SocketTemp2_Cri)
	{
		LISTVIEW_SetItemText(hItem, 3, 1, "过温");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 3, 1, "正常");
	}

    /**< A枪输出电流 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_AC_A_CurrUp_Cri) == defSignalCON_Alarm_AC_A_CurrUp_Cri)
	{
		LISTVIEW_SetItemText(hItem, 5, 1,"过流");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 5, 1, "正常");
	}

	/**< RFID */
	if ((pEVSE->status.ulSignalFault & defSignalEVSE_Fault_RFID) == defSignalEVSE_Fault_RFID)
	{
		LISTVIEW_SetItemText(hItem, 1, 2, "故障");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 1, 2, "正常");
	}

	/**< A枪枪锁 */
	if ((pcont->status.ulSignalState & defSignalCON_State_SocketLock) == defSignalCON_State_SocketLock)
	{
		LISTVIEW_SetItemText(hItem, 3, 2, "锁止");
	}
	else
	{
        LISTVIEW_SetItemText(hItem, 3, 2,"解锁");
	}

    /**< 枪N温度 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_AC_N_Temp_Cri) == defSignalCON_Alarm_AC_N_Temp_Cri)
	{
		LISTVIEW_SetItemText(hItem, 5, 2, "过温");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 5, 2, "正常");
	}

	/**< 交流电压 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_AC_A_VoltUp) == defSignalCON_Alarm_AC_A_VoltUp)
	{
		LISTVIEW_SetItemText(hItem, 1, 3,"过压");
	}
	else if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_AC_A_VoltLow) == defSignalCON_Alarm_AC_A_VoltLow)
	{
		LISTVIEW_SetItemText(hItem, 1, 3,"欠压");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 1, 3, "正常");
	}

	/**< 频率 */
	if ((pcont->status.ulSignalAlarm & defSignalCON_Alarm_AC_A_Freq_Cri) == defSignalCON_Alarm_AC_A_Freq_Cri)
	{
		LISTVIEW_SetItemText(hItem, 3, 3, "异常");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 3, 3, "正常");
	}

    /**< 防雷器状态 */
	if ((pEVSE->status.ulSignalAlarm & defSignalEVSE_Alarm_Arrester) == defSignalEVSE_Alarm_Arrester)
	{
		LISTVIEW_SetItemText(hItem, 5, 3, "故障");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 5, 3, "正常");
	}

	/**< 输出继电器状态 */
	if ((pcont->status.ulSignalState & defSignalCON_State_AC_A_Relay) == defSignalCON_State_AC_A_Relay)
	{
		LISTVIEW_SetItemText(hItem, 1, 4, "故障");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 1, 4, "正常");
	}

	/**< 控制导引状态 */
	if ((pcont->status.ulSignalFault & defSignalCON_Fault_CP) == defSignalCON_Fault_CP)
	{
		LISTVIEW_SetItemText(hItem, 3, 4, "故障");
	}
	else
	{
		LISTVIEW_SetItemText(hItem, 3, 4, "正常");
	}
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE *pMsg)
{
    const void *pData;
    volatile WM_HWIN      hItem;
    U32          FileSize;
    int          NCode;
    int          Id;
    uint16_t     i,_strNum[3];
	volatile HEADER_Handle hHeader;
	char    buf[20];
//	static char  Value = 0;
    SCROLLBAR_Handle hScroll;
    SCROLLBAR_Handle wScroll;
    CON_t	*pcont;

//    static int _x,_y;
//    WM_SCROLL_STATE ScrollState;

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        WM_SetFocus(_hWinManagerAlarmLog);
        /// TODO (zshare#1#): 下面的if不起作用.\
        但是if里嵌套的if起作用,目前先用此来规避不起作用的if
        if(_hWinManagerAlarmLog == cur_win)
        {
            /**< 数据处理 */
            //Data_Process(pMsg);
            /**< 信号数据处理 */
            Signal_Show();
            /**< 灯光控制 */
            Led_Show();
            /**< 如果界面发生了切换 */
            if(_hWinManagerAlarmLog == cur_win)
            {
                /**< 故障分析 */
                //Err_Analy(pMsg->hWin);
                /**< 特殊触控点分析 */
                CaliDone_Analy(pMsg->hWin);
            }
        }
        break;
    case WM_INIT_DIALOG:
        //
        // 初始列表控件
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        /* 设置列表控件中header控件的所显示文本的字体 */
        hHeader = LISTVIEW_GetHeader(hItem);
        HEADER_SetFont(hHeader, &SIF16_Font);

        /*srollbar*/
        hScroll = SCROLLBAR_CreateAttached(hItem, 0);//水平滑轮
        SCROLLBAR_SetNumItems(hScroll, 30 * 4);
        SCROLLBAR_SetWidth(hScroll,20);
        wScroll = SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);//垂直滑轮
        SCROLLBAR_SetNumItems(wScroll, 30 * 20);
        SCROLLBAR_SetWidth(wScroll,20);
        /*end*/

        /* 设置列表控件选项中所显示文本的字体 */
        LISTVIEW_SetFont(hItem, &SIF16_Font);
        /* 设置列表控件表格可见 */
        LISTVIEW_SetGridVis(hItem, 1);

        column_num = LISTVIEW_GetNumColumns(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
        for(i = 0;i < column_num;i++)
        {
            LISTVIEW_DeleteColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),0);
        }
        /*状态量   状态值     状态量     状态值     状态量     状态值*/
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, stateName, GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, stateValue, GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, stateName, GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, stateValue, GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, stateName, GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, stateValue, GUI_TA_HCENTER | GUI_TA_VCENTER);

        LISTVIEW_AddRow(hItem, NULL);//增加一行
        /**< 工作状态 */
        LISTVIEW_SetItemText(hItem, 0, 0, stateWork);
        /**< 急停状态 */
        LISTVIEW_SetItemText(hItem, 2, 0, stateScram);
        /**< 电表通信 */
        LISTVIEW_SetItemText(hItem, 4, 0, stateMeter);
        LISTVIEW_AddRow(hItem, NULL);//增加一行
        /**< A插座温度1 */
        LISTVIEW_SetItemText(hItem, 0, 1, stateSocket1Temp);
        /**< A插座温度2 */
        LISTVIEW_SetItemText(hItem, 2, 1, stateSocket2Temp);
        /**< A枪输出电流 */
        LISTVIEW_SetItemText(hItem, 4, 1, stateCurrent);
        LISTVIEW_AddRow(hItem, NULL);//增加一行
        /**< RFID */
        LISTVIEW_SetItemText(hItem, 0, 2, stateRFID);
        /**< A枪枪锁 */
        LISTVIEW_SetItemText(hItem, 2, 2, stateSocketLock);
        /**< 枪N温度 */
        LISTVIEW_SetItemText(hItem, 4, 2, stateNTemp);
        LISTVIEW_AddRow(hItem, NULL);//增加一行
        /**< 交流电压 */
        LISTVIEW_SetItemText(hItem, 0, 3, stateACVol);
        /**< 频率 */
        LISTVIEW_SetItemText(hItem, 2, 3, stateACFreq);
        /**< 防雷器状态 */
        LISTVIEW_SetItemText(hItem, 4, 3, stateScram);
        LISTVIEW_AddRow(hItem, NULL);//增加一行
        /**< 输出继电器状态 */
        LISTVIEW_SetItemText(hItem, 0, 4, stateRelay);
        /**< 控制导引状态 */
        LISTVIEW_SetItemText(hItem, 2, 4, stateCP);
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {

        break;
    }
    case WM_TIMER:
        if(pMsg->Data.v == _timerRTC)
        {
            /**< 显示时间和日期 */
            Caculate_RTC_Show(pMsg, ID_TEXT_1, ID_TEXT_2);
           // TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_3), strCSQ);
            /**< 重启定时器 */
            WM_RestartTimer(pMsg->Data.v, 20);
        }
        if(pMsg->Data.v == _timerSignal)
        {

            WM_RestartTimer(pMsg->Data.v, 2000);
        }
        if(pMsg->Data.v == _timerData)
        {
            Status_Content_Analy(pMsg);
            WM_RestartTimer(pMsg->Data.v,3000);
        }
        break;
    case MSG_CREATERRWIN:
        /**< 故障界面不存在则创建,存在则刷新告警 */
        err_window(pMsg->hWin);
        break;
    case MSG_DELERRWIN:
        /**< 故障界面存在则删除故障界面 */
        if(bittest(winCreateFlag,0))
        {
            bitclr(winCreateFlag,0);
            GUI_EndDialog(err_hItem,0);
            err_hItem = 0;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/** @brief
 *  输出管理员界面里的信息状态量部分
 * @param
 * @param
 * @return
 *       CreateManagerInfoStatus
*/
WM_HWIN CreateManagerInfoStatus(WM_HWIN srcHwin)
{
    _hWinManagerInfoStatus = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, srcHwin, 0, 0);
    _timerRTC = WM_CreateTimer(WM_GetClientWindow(_hWinManagerInfoAnalog), ID_TimerTime, 20, 0);
    _timerData = WM_CreateTimer(WM_GetClientWindow(_hWinManagerInfoAnalog), ID_TimerFlush,1000,0);
//    _timerSignal = WM_CreateTimer(WM_GetClientWindow(_hWinManagerInfoAnalog), ID_TimerSignal,5000,0);
    return 0;
}
/*************************** End of file ****************************/
