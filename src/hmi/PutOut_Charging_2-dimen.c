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
#define ID_BUTTON_0     (GUI_ID_USER + 0x1B)
#define ID_IMAGE_0  (GUI_ID_USER + 0x0C)

#define ID_IMAGE_0_IMAGE_0  0x00
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
*       _aDialogCharging2dimen
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCharging2dimen[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 789, 459, 0, 0, 0 },
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
    { TEXT_CreateIndirect, "Text", ID_TEXT_18, 305, 320, 300, 30, 0, 0x0, 0 },
//    { IMAGE_CreateIndirect, "二维码", ID_IMAGE_0, 55, 291, 101, 87, 0, 0, 0 },
    // USER END
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _acImage_0, "BMP", ID_IMAGE_0_IMAGE_0
*/
static const U8 _acImage_0[463] = {
  0x42, 0x4D, 0xCE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00,
  0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21,
  0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00,
  0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84,
  0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08,
  0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10,
  0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00,
  0x88, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42,
  0x10, 0x84, 0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84,
  0x21, 0x08, 0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0x88, 0x42, 0x10, 0x84, 0x21, 0x08,
  0x40, 0x00, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x40, 0x00, 0xC2, 0x10, 0x84, 0x21, 0x08, 0x42, 0x40, 0x00, 0xA1, 0x08, 0x42, 0x10, 0x84, 0x21, 0x40, 0x00, 0x90, 0x84, 0x21, 0x08, 0x42, 0x10, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
  0x00,
};
/*********************************************************************
*
*       _GetImageById
*/
static const void * _GetImageById(U32 Id, U32 * pSize) {
  switch (Id) {
  case ID_IMAGE_0_IMAGE_0:
    *pSize = sizeof(_acImage_0);
    return (const void *)_acImage_0;
  }
  return NULL;
}
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
static void Caculate_RTC(WM_MESSAGE *pMsg)
{
    static uint8_t num = 0;
    uint8_t Timer_buf[10];

    WM_HWIN hWin = pMsg->hWin;

    Caculate_RTC_Show(pMsg,ID_TEXT_1,ID_TEXT_2);

    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), "14");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), "5");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_2), "1.5");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_3), "45.6");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_4), "00");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_5), "02");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_6), "34");

    xsprintf((char *)Timer_buf, "%02dS", wait_timer.charge_screen_lock);
    if((num++) >= 3)
    {
        num = 0;
        if((wait_timer.charge_screen_lock--) == 0)
        {
            wait_timer.charge_screen_lock = 0;
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_18), "屏幕已锁定，请在APP上操作");
            //进行退出的跳页操作
        }
    }
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_7), Timer_buf);
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

        //
        // Initialization of '二维码'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
        IMAGE_SetBMP(hItem, pData, FileSize);

        //
        // Initialization of 'Text'
        //
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5),&XBF36_Font,GUI_BLACK, "正在充电 ... ...");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6),&XBF24_Font,GUI_BLACK, "已充电");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_7),&XBF24_Font,GUI_BLACK, "充入电量");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_8),&XBF24_Font,GUI_BLACK, "当前服务费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_9),&XBF24_Font,GUI_BLACK, "当前电费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_10),&XBF24_Font,GUI_BLACK, "消费总额");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_11),&XBF24_Font,GUI_BLACK, "度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_12),&XBF24_Font,GUI_BLACK, "元/单");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_13),&XBF24_Font,GUI_BLACK, "元/度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_14),&XBF24_Font,GUI_BLACK, "元");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_15),&XBF24_Font,GUI_BLACK, "小时");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_16),&XBF24_Font,GUI_BLACK, "分");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_17),&XBF24_Font,GUI_BLACK, "秒");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_18),&XBF24_Font,GUI_RED, "后屏幕锁定");
        //
        // Initialization of 'Edit'
        //
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_6),&XBF24_Font,"00");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_7),&XBF24_Font,"00S");
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0),GUI_TA_LEFT|GUI_TA_VCENTER,
                    &XBF24_Font,BUTTON_CI_UNPRESSED,GUI_BLUE,BUTTON_CI_UNPRESSED,GUI_BLUE,"停止充电");
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
                PutOut_Home();
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
*       CreateCharging2dimen
*/
WM_HWIN CreateCharging2dimen(void);
WM_HWIN CreateCharging2dimen(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCharging2dimen, GUI_COUNTOF(_aDialogCharging2dimen), _cbDialog, WM_HBKWIN, 0, 0);
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
void PutOut_Charging_2dimen()
{
    wait_timer.charge_screen_lock = 60;
    CreateCharging2dimen();
    while(1)
    {
       dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
        GUI_Delay(10);
    }
}
// USER END

/*************************** End of file ****************************/




