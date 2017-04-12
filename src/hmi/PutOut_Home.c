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
#define ID_BUTTON_1     (GUI_ID_USER + 0x04)
#define ID_TEXT_0     (GUI_ID_USER + 0x08)
#define ID_IMAGE_0     (GUI_ID_USER + 0x0A)

#define ID_IMAGE_0_IMAGE_0     0x00

// USER START (Optionally insert additional defines)
#define ID_TEXT_1     (GUI_ID_USER + 0x0B)
#define ID_TEXT_2     (GUI_ID_USER + 0x0C)
#define ID_TEXT_3     (GUI_ID_USER + 0x0D)
#define ID_TEXT_4     (GUI_ID_USER + 0x0E)

#define ID_TEXT_5     (GUI_ID_USER + 0x05)
#define ID_EDIT_0     (GUI_ID_USER + 0x06)
#define ID_TEXT_6     (GUI_ID_USER + 0x07)
#define ID_TEXT_7     (GUI_ID_USER + 0x02)
#define ID_EDIT_1     (GUI_ID_USER + 0x03)
#define ID_TEXT_8     (GUI_ID_USER + 0x09)

#define ID_TimerTime    0
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _acImage_0, "BMP", ID_IMAGE_0_IMAGE_0
*/
static const U8 _acImage_0[463] =
{
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

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 67, 186, 250, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 404, 186, 250, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 245, 99, 254, 50, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 114, 299, 50, 50, 0, 0, 0 },
    // USER START (Optionally insert additional widgets)
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 630, 0, 80, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 720, 0, 70, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 540, 0, 90, 16, 0, 0x0, 0 },//网络信号强度
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 225, 367, 300, 20, 0, 0x0, 0 },//最底端的说明
    { TEXT_CreateIndirect, "Text", ID_TEXT_5, 422, 247, 80, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 510, 247, 80, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_6, 598, 247, 80, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_7, 422, 286, 80, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 510, 286, 80, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_8, 598, 286, 80, 30, 0, 0x0, 0 },
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
*       _GetImageById
*/
static const void *_GetImageById(U32 Id, U32 *pSize)
{
    switch (Id)
    {
    case ID_IMAGE_0_IMAGE_0:
        *pSize = sizeof(_acImage_0);
        return (const void *)_acImage_0;
    }
    return NULL;
}

// USER START (Optionally insert additional static code)
static void Timer_Process(WM_MESSAGE *pMsg)
{
    uint8_t i = 0;
    WM_HWIN hWin = pMsg->hWin;

    Caculate_RTC_Show(pMsg,ID_TEXT_1,ID_TEXT_2);

    //需要增加3G模块的信号强度判断
    switch(i % 5)
    {
    case 0:
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "信号:非常强");
        break;
    case 1:
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "信号:一般");
        break;
    case 2:
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "信号:差");
        break;
    case 3:
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "信号:极差");
        break;
    default:
        TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "信号:无");
        break;
    }

    //充电费和服务费的费用值显示
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), "45");
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), "56");
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
    static WM_HTIMER   htimer;
    U32          FileSize;
    int          NCode;
    int          Id;
            uint8_t *buf = "56";
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //

        FrameWin_Init(pMsg,ID_TEXT_1,ID_TEXT_2,ID_TEXT_3,ID_TEXT_4);
        //
        // Initialization of 'Image'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
        IMAGE_SetBMP(hItem, pData, FileSize);
        //
        // Initialization of 'Edit'
        //
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0),&XBF24_Font,"12");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1),&XBF24_Font,"23");
        // Initialization of 'Text'
        //
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0),&XBF36_Font,GUI_BLACK, "请选择支付方式");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5),&XBF24_Font,GUI_BLACK, "充电费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6),&XBF24_Font,GUI_BLACK, "元/度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_7),&XBF24_Font,GUI_BLACK, "服务费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_8),&XBF24_Font,GUI_BLACK, "元/度");
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0),GUI_TA_LEFT|GUI_TA_VCENTER,
                    &XBF24_Font,BUTTON_CI_UNPRESSED,GUI_BLUE,BUTTON_CI_UNPRESSED,GUI_BLUE,"手机支付请扫描二维码");
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &XBF24_Font,BUTTON_CI_UNPRESSED,GUI_BLUE,BUTTON_CI_UNPRESSED,GUI_BLUE,"刷卡支付请刷卡");
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
        case ID_BUTTON_1: // Notifications sent by 'Button'
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
        //Timer_Process(pMsg);
        //EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), "45");

        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), buf);
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
*       CreateFramewin
*/
WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    WM_CreateTimer(WM_GetClientWindow(hWin), ID_TimerTime, 1000, 0);

//    WM_CreateTimer(hWin, ID_TimerTime, 1000, 0);

    return hWin;
}

// USER START (Optionally insert additional public code)
/** @brief
 *  输出充电桩欢迎界面
 * @param
 * @param
 * @return
 *
 */

void PutOut_Home()
{
    CreateFramewin();
    dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
    while(1)
    {
        //i++;
        //GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        //GUI_SetFont(&XBF16_Font);
        //GUI_SetColor(GUI_RED);
        // GUI_DispStringAt("        ",633,20);
        // GUI_Delay(10);
        // GUI_DispStringAt((const char*)Date_buf,633,20);
        //GUI_DispCEOL();

        //GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        // GUI_SetFont(&XBF16_Font);
        // GUI_SetColor(GUI_RED);
        // GUI_DispStringAt("        ",720,20);
        // GUI_Delay(10);
        // GUI_DispStringAt((const char*)Time_buf,720,20);
        // GUI_DispCEOL();

        GUI_Delay(10);
    }
}
// USER END

/*************************** End of file ****************************/

