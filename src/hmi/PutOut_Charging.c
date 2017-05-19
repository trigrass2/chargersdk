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
#include "xbffontcreate.h"
#include "touchtimer.h"
#include "interface.h"
#include "HMI_Start.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_BUTTON_0     (GUI_ID_USER + 0x1B)
// USER START (Optionally insert additional defines)
#define ID_TEXT_1     (GUI_ID_USER + 0x01)
#define ID_TEXT_2     (GUI_ID_USER + 0x02)
#define ID_TEXT_3     (GUI_ID_USER + 0x03)
#define ID_TEXT_4     (GUI_ID_USER + 0x04)

#define ID_TEXT_5     (GUI_ID_USER + 0x05)
#define ID_TEXT_6     (GUI_ID_USER + 0x06)
#define ID_TEXT_7     (GUI_ID_USER + 0x07)
#define ID_TEXT_8     (GUI_ID_USER + 0x08)
#define ID_TEXT_9     (GUI_ID_USER + 0x09)
#define ID_TEXT_10    (GUI_ID_USER + 0x0A)
#define ID_EDIT_0     (GUI_ID_USER + 0x0B)
#define ID_EDIT_1     (GUI_ID_USER + 0x0C)
#define ID_EDIT_2     (GUI_ID_USER + 0x0D)
#define ID_EDIT_3     (GUI_ID_USER + 0x0E)
#define ID_TEXT_11    (GUI_ID_USER + 0x0F)
#define ID_TEXT_12    (GUI_ID_USER + 0x10)
#define ID_TEXT_13    (GUI_ID_USER + 0x11)
#define ID_TEXT_14    (GUI_ID_USER + 0x12)
#define ID_EDIT_4     (GUI_ID_USER + 0x13)
#define ID_TEXT_15    (GUI_ID_USER + 0x14)
#define ID_EDIT_5     (GUI_ID_USER + 0x15)
#define ID_TEXT_16    (GUI_ID_USER + 0x16)
#define ID_EDIT_6     (GUI_ID_USER + 0x17)
#define ID_TEXT_17    (GUI_ID_USER + 0x18)
#define ID_EDIT_7     (GUI_ID_USER + 0x19)
#define ID_TEXT_18    (GUI_ID_USER + 0x1A)

#define ID_TimerTime    0
// USER END
volatile uint8_t countdown_flag;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCharging
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCharging[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    // USER START (Optionally insert additional widgets)
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 630, 0, 80, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 720, 0, 70, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 540, 0, 90, 16, 0, 0x0, 0 },//网络信号强度
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 225, 367, 300, 20, 0, 0x0, 0 },//最底端的说明
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 627, 342, 100, 36, 0, 0x0, 0 },//停止充电
    { TEXT_CreateIndirect, "Text", ID_TEXT_5, 222, 40, 326, 35, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_6, 190, 100, 120, 35, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_7, 222, 159, 100, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_8, 222, 199, 120, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_9, 222, 237, 100, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_10, 222, 275, 100, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 360, 159, 88, 30, 0, 0x64, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 360, 199, 88, 30, 0, 0x64, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 360, 237, 88, 30, 0, 0x64, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_3, 360, 275, 88, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_11, 460, 159, 80, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_12, 460, 199, 80, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_13, 460, 237, 80, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_14, 460, 275, 80, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_4, 290, 100, 45, 35, 0, 0x64, 0 },//小时数值
    { TEXT_CreateIndirect, "Text", ID_TEXT_15, 335, 100, 70, 35, 0, 0x0, 0 },//小时
    { EDIT_CreateIndirect, "Edit", ID_EDIT_5, 405, 100, 45, 35, 0, 0x64, 0 },//分钟数值
    { TEXT_CreateIndirect, "Text", ID_TEXT_16, 450, 100, 70, 35, 0, 0x0, 0 },//分钟
    { EDIT_CreateIndirect, "Edit", ID_EDIT_6, 500, 100, 45, 35, 0, 0x64, 0 },//秒数值
    { TEXT_CreateIndirect, "Text", ID_TEXT_17, 545, 100, 70, 35, 0, 0x0, 0 },//秒钟
    { EDIT_CreateIndirect, "Edit", ID_EDIT_7, 250, 320, 45, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_18, 305, 320, 260, 30, 0, 0x0, 0 },

    // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
static void Caculate_RTC(WM_MESSAGE *pMsg)
{
    static uint8_t timer_count = 0;
    uint8_t Timer_buf[10];
    uint8_t temp_buf[32];
    CON_t *pCON;
    time_t now;
    uint32_t diffsec;
    volatile uint8_t sec;
    uint8_t min;
    uint8_t hour;
    EventBits_t uxBitCharge;
    EventBits_t uxBitHMI;
    EventBits_t uxBitIsDone;
    static volatile uint8_t sec_done = 0;
    static time_t first;
    static uint8_t first_flag = 0;

    WM_HWIN hWin = pMsg->hWin;
    CaliDone_Analy(hWin);
    Caculate_RTC_Show(pMsg, ID_TEXT_1, ID_TEXT_2);

    pCON = CONGetHandle(0);
    //再次刷卡事件发生,进行解锁操作
    uxBitHMI = xEventGroupWaitBits(xHandleEventHMI,
                    defEventBitHMI_RFIDOLD,
                    pdTRUE, pdTRUE, 0);
    if((uxBitHMI & defEventBitHMI_RFIDOLD) == defEventBitHMI_RFIDOLD)
    {
        first_flag = 0;
        sec_done = 0;
        bitclr(calebrate_done,2);
    }

    uxBitHMI = xEventGroupWaitBits(xHandleEventHMI,
                                   defEventBitHMI_ChargeReqDispDone,
                                   pdTRUE, pdTRUE, 0);
    if((uxBitHMI & defEventBitHMI_ChargeReqDispDone) == defEventBitHMI_ChargeReqDispDone)
    {
        xEventGroupSetBits(xHandleEventHMI,defeventBitHMI_ChargeReqDispDoneOK);
        PutOut_Charge_Done();
    }

    pCON = CONGetHandle(0);///** @todo (zshare#1#): 双枪ID选择 */
    now = time(NULL);
    if(first_flag == 0)
    {
        first_flag = 1;
        first = now;
    }
    diffsec = (uint32_t)difftime(now, pCON->order.tStartTime);
    if(diffsec > 86400)
    {
        diffsec = 86400;
    }
    hour = diffsec / 3600;
    min = diffsec % 3600 / 60;
    sec = diffsec % 3600 % 60;

    sprintf(temp_buf, "%02d", hour);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_4), temp_buf);//已充电时间小时
    sprintf(temp_buf, "%02d", min);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_5), temp_buf);// min
    sprintf(temp_buf, "%02d", sec);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_6), temp_buf);// sec

    sprintf(temp_buf, "%.2lf",  pCON->order.dTotalPower);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), temp_buf);//充入电量
    sprintf(temp_buf, "%.2lf", pCON->order.dTotalServiceFee);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), temp_buf);//当前服务费
    sprintf(temp_buf, "%.2lf", pCON->order.dTotalPowerFee);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_2), temp_buf);//当前电费
    sprintf(temp_buf, "%.2lf", pCON->order.dTotalFee);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_3), temp_buf);//消费总额

    diffsec = (uint32_t)difftime(now, first);
    if(diffsec > 86400)
    {
        diffsec = 86400;
    }
    hour = diffsec / 3600;
    min = diffsec % 3600 / 60;
    sec = diffsec % 3600 % 60;

    if(sec == 59)
    {
        sec_done = 1;
    }

    if(sec_done == 1)
    {
        EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_7), "00S");
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_18), "屏幕已锁定，操作请刷卡");
        xEventGroupSetBits(xHandleEventHMI,defEventBitHMI_ChargeReqLockLcdOK);
        bitset(calebrate_done,2);
    }
    else
    {
        bitclr(calebrate_done,2);
        xsprintf((char *)Timer_buf, "%02dS", (60-sec));
        EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_7), Timer_buf);
    }

    ErrWindow_Show(hWin);
//    //等待结束充电信号
//    uxBitHMI = xEventGroupWaitBits(xHandleEventHMI,
//                   defEventBitHMI_ChargeReqDoneOK,
//                   pdTRUE, pdTRUE, 0);
//    if((uxBitHMI & defEventBitHMI_ChargeReqDoneOK) == defEventBitHMI_ChargeReqDoneOK)
//    {
//        WM_DeleteWindow(hWin);
//        PutOut_Home();
//    }
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE *pMsg)
{
    const void *pData;
    WM_HWIN      hItem;
    U32          FileSize;
    int          NCode;
    int          Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //
        FrameWin_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4);
        //
        // Initialization of 'Text'
        //
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5), &XBF36_Font, GUI_BLACK, "正在充电 ... ...");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6), &XBF24_Font, GUI_BLACK, "已充电");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_7), &XBF24_Font, GUI_BLACK, "充入电量");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_8), &XBF24_Font, GUI_BLACK, "当前服务费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_9), &XBF24_Font, GUI_BLACK, "当前电费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_10), &XBF24_Font, GUI_BLACK, "消费总额");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_11), &XBF24_Font, GUI_BLACK, "度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_12), &XBF24_Font, GUI_BLACK, "元/单");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_13), &XBF24_Font, GUI_BLACK, "元/度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_14), &XBF24_Font, GUI_BLACK, "元");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_15), &XBF24_Font, GUI_BLACK, "小时");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_16), &XBF24_Font, GUI_BLACK, "分");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_17), &XBF24_Font, GUI_BLACK, "秒");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_18), &XBF24_Font, GUI_RED, "后屏幕锁定");
        //
        // Initialization of 'Edit'
        //
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_6), &XBF24_Font, "00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_7), &XBF24_Font, "00S");
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), GUI_TA_LEFT | GUI_TA_VCENTER,
                    &XBF24_Font, BUTTON_CI_UNPRESSED, GUI_BLUE, BUTTON_CI_UNPRESSED, GUI_BLUE, "停止充电");
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id)
        {
        case ID_BUTTON_0: // Notifications sent by 'Button'
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                //处于解锁状态下要发送停止充电事件
                if(bittest(calebrate_done,2) == 0)
                {
                    xEventGroupSetBits(xHandleEventHMI,defEventBitHMI_ChargeReqClickOK);
                }
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
            // USER START (Optionally insert additional code for further Ids)
            // USER END
        }
        break;
        // USER START (Optionally insert additional message handling)
    case WM_TIMER:
        /* 显示时间和日期 */
        Caculate_RTC(pMsg);
        /* 重启定时器 */
        WM_RestartTimer(pMsg->Data.v, REFLASH);
        break;
        // USER END
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
/*********************************************************************
*
*       CreateCharging
*/
WM_HWIN CreateCharging(void);
WM_HWIN CreateCharging(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCharging, GUI_COUNTOF(_aDialogCharging), _cbDialog, WM_HBKWIN, 0, 0);
    WM_CreateTimer(WM_GetClientWindow(hWin), ID_TimerTime, 1000, 0);

    return hWin;
}

// USER START (Optionally insert additional public code)

/** @brief
 *  输出正在充电界面
 * @param
 * @param
 * @return
 *
 */
void PutOut_Charging()
{
    WM_HWIN hWin;

    wait_timer.charge_screen_lock = 60;
    hWin = CreateCharging();
    countdown_flag = 0;
    led_ctrl(1,green,breath);
    while(1)
    {
        GUI_Delay(1);
        dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
        vTaskDelay(20);
    }
}
// USER END

/*************************** End of file ****************************/




