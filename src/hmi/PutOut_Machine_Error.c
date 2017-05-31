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
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)
#define ID_IMAGE_0      (GUI_ID_USER + 0x09)
// USER START (Optionally insert additional defines)
#define ID_TEXT_1     (GUI_ID_USER + 0x0B)
#define ID_TEXT_2     (GUI_ID_USER + 0x0C)
#define ID_TEXT_3     (GUI_ID_USER + 0x0D)
#define ID_TEXT_4     (GUI_ID_USER + 0x0E)
#define ID_TEXT_5     (GUI_ID_USER + 0x05)
#define ID_TEXT_6     (GUI_ID_USER + 0x02)

#define ID_TimerTime    0
// USER END

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
*       _aDialogMachineError
*/
static const GUI_WIDGET_CREATE_INFO _aDialogMachineError[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 789, 459, 0, 0, 0 },
    // USER START (Optionally insert additional widgets)
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 630, 0, 80, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 720, 0, 70, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 540, 0, 90, 16, 0, 0x0, 0 },//网络信号强度
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 225, 367, 300, 20, 0, 0x0, 0 },//最底端的说明
    { TEXT_CreateIndirect, "Text", ID_TEXT_6, 286, 162, 429, 61, 0, 0x0, 0 },
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
    uint8_t Timer_buf[10];
    static uint8_t num = 0;

    WM_HWIN hWin = pMsg->hWin;

    Caculate_RTC_Show(pMsg,ID_TEXT_1,ID_TEXT_2);

    xsprintf((char *)Timer_buf, "(%02dS)", wait_timer.card_valid);
    if((num++) >= (1000 / REFLASH))
    {
        num = 0;
        if((wait_timer.card_info--) == 0)
        {
            wait_timer.card_info = 0;
            //进行退出的跳页操作
        }
    }
    TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5), Timer_buf);
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
        FrameWin_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4,ID_IMAGE_0);

        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6),&XBF36_Font,GUI_RED, "设 备 故 障");
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
        WM_RestartTimer(pMsg->Data.v, 20);
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
*       CreateFramewin_MachineError
*/
WM_HWIN CreateFramewin_MachineError(void);
WM_HWIN CreateFramewin_MachineError(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogMachineError, GUI_COUNTOF(_aDialogMachineError), _cbDialog, WM_HBKWIN, 0, 0);
    WM_CreateTimer(WM_GetClientWindow(hWin), ID_TimerTime, 1000, 0);

    return hWin;
}

// USER START (Optionally insert additional public code)

/** @brief
 *  输出设备故障界面
 * @param
 * @param
 * @return
 *
 */
void PutOut_Machine_Error()
{
    CreateFramewin_MachineError();
    while(1)
    {
        dispbmp("system/charge.bmp", 0, 250, 100, 1, 1);
        //GUI_Delay(10);
        dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
        GUI_Delay(10);
    }
}
// USER END

/*************************** End of file ****************************/



