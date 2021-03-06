/*********************************************************************
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
#define ID_WINDOW_0     (GUI_ID_USER + 0x10)
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_1     (GUI_ID_USER + 0x04)
#define ID_BUTTON_MANAGER   (GUI_ID_USER + 0x12)
#define ID_TEXT_0     (GUI_ID_USER + 0x11)
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
#define ID_TEXT_9     (GUI_ID_USER + 0x0F)
#define ID_BUTTON_2  (GUI_ID_USER + 0x13)
#define ID_TEXT_10     (GUI_ID_USER + 0x14)
#define ID_TimerTime    0

//14行1列，14个故障项
#define TEXT_MAX_X 1
#define TEXT_MAX_Y 14
#define ERROR_LINE 14
#define ERROR_CAL 1
//后续将编辑和文本的滚轮方式用链表进行封装
static EDIT_Handle _aahEdit[TEXT_MAX_Y][TEXT_MAX_X];
static TEXT_Handle _aahText[ERROR_LINE][ERROR_CAL];
static BUTTON_Handle _framebutton;
static int _x,_y;
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
    //{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { WINDOW_CreateIndirect, "Window_test", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 67, 186, 250, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 404, 186, 250, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 245, 99, 254, 50, 0, 0x0, 0 },
    //{ IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 789, 459, 0, 0, 0 },//尝试bmp单独显示
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 114, 299, 50, 50, 0, 0, 0 },
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
    { TEXT_CreateIndirect, "Text", ID_TEXT_10, 200, 0, 240, 30, 0, 0x0, 0 },
//    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 750, 0, 40, 40, 0, 0x0, 0 },
//    { BUTTON_CreateIndirect, "Button", ID_BUTTON_MANAGER, 200, -43, 300, 50, 0, 0x0, 0 },
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
static void _cbWindow(WM_MESSAGE *pMsg) {
    WM_SCROLL_STATE ScrollState;
    WM_HWIN      hItem;
    char *dest = (char *)malloc(10);
    static int flag = 0;
    switch (pMsg->MsgId) {
        case WM_CREATE:
//            WINDOW_SetDefaultBkColor(GUI_GRAY);
        break;
        case WM_NOTIFY_PARENT:
            if(pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED)
            {
                if(WM_GetId(pMsg->hWinSrc) == GUI_ID_HSCROLL)
                {
                    /* 得到滚动条的状态，得到的数值好像是负值 才能使得 _x - ScrollState.v是正值 */
                    WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
                    if (_x != ScrollState.v)
                    {
                        int x, y;
                        for (y = 0; y < TEXT_MAX_Y; y++)
                        {
                            for (x = 0; x < TEXT_MAX_X; x++)
                            {
                                WM_MoveWindow(_aahEdit[y][x], _x - ScrollState.v, 0);
                            }
                        }
                    _x = ScrollState.v;
                    }
                }
                else if(WM_GetId(pMsg->hWinSrc) == GUI_ID_VSCROLL)
                {
                    WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
                    if (_y != ScrollState.v)
                    {
                        int x, y;
                        for (y = 0; y < TEXT_MAX_Y; y++)
                        {
                            for (x = 0; x < TEXT_MAX_X; x++)
                            {
                                WM_MoveWindow(_aahEdit[y][x],0, _y - ScrollState.v);
                            }
                        }
                        _y = ScrollState.v;
                    }
                }
                else
                {
                    EDIT_SetText(_aahEdit[2][4],"adsfa");
                    flag = EDIT_GetValue(_aahEdit[2][3]);
                    EDIT_GetText(_aahEdit[2][4],dest,10);
                    flag = EDIT_GetValue(_aahEdit[2][3]);
                }
            }
            break;
        default:
        WM_DefaultProc(pMsg);
    }
}
static void _cbDialog_Error()
{
    int x, y;
    WM_HWIN hWindow;
    SCROLLBAR_Handle hScroll;
    SCROLLBAR_Handle wScroll;
    uint8_t tmp[20] = {"交流输入故障"};
    static int flag = 0;

    if(flag == 0)
    {
        flag = 1;
        // USER START (Optionally insert additional variables)
        // 创建窗口
        hWindow = WM_CreateWindow(495, 145, 300, 250, WM_CF_SHOW, &_cbWindow, 0);

        //WINDOW_SetBkColor(hWindow,GUI_GRAY);
        //创建水平滑轮
//        hScroll = SCROLLBAR_CreateAttached(hWindow, 0);//水平滑轮
//        //设置滑轮条目数量
//        SCROLLBAR_SetNumItems(hScroll, 48 * TEXT_MAX_X);
//        //设置页尺寸
//        //SCROLLBAR_SetPageSize(hScroll, 220);
//        SCROLLBAR_SetValue(hScroll,10);
        //创建垂直滑轮
        wScroll = SCROLLBAR_CreateAttached(hWindow, SCROLLBAR_CF_VERTICAL);//垂直滑轮
        //设置滑轮条目数量
        SCROLLBAR_SetNumItems(wScroll, 80 * TEXT_MAX_Y);
        //设置页尺寸
        SCROLLBAR_SetPageSize(wScroll, 220);
        //创建文本区 -- 24号字体 4-96 5-120 6-144 7-168 8-192
        _aahText[0][0] = TEXT_CreateEx(30, 20, 24*strlen(tmp), 25,hWindow,WM_CF_SHOW,0,13,tmp);
        _aahText[1][0] = TEXT_CreateEx(30, 50, 120, 25,hWindow,WM_CF_SHOW,0,13,"充电电流过大过大过大过大过大！");
        _aahText[2][0] = TEXT_CreateEx(30, 80, 120, 25,hWindow,WM_CF_SHOW,0,13,"环境温度:");
        _aahText[3][0] = TEXT_CreateEx(30, 110, 144, 25,hWindow,WM_CF_SHOW,0,13,"A插座温度:");
        _aahText[4][0] = TEXT_CreateEx(30, 140, 144, 25,hWindow,WM_CF_SHOW,0,13,"B插座温度:");
        _aahText[5][0] = TEXT_CreateEx(30, 170, 168, 25,hWindow,WM_CF_SHOW,0,13,"A枪输出电流:");
        _aahText[6][0] = TEXT_CreateEx(30, 200, 168, 25,hWindow,WM_CF_SHOW,0,13,"B枪输出电流:");
        _aahText[7][0] = TEXT_CreateEx(30, 230, 120, 25,hWindow,WM_CF_SHOW,0,13,"A枪枪锁:");
        _aahText[8][0] = TEXT_CreateEx(30, 260, 120, 25,hWindow,WM_CF_SHOW,0,13,"B枪枪锁:");
        _aahText[9][0] = TEXT_CreateEx(30, 290, 120, 25,hWindow,WM_CF_SHOW,0,13,"交流电压:");
        _aahText[10][0] = TEXT_CreateEx(30, 320, 120, 25,hWindow,WM_CF_SHOW,0,13,"交流电流:");
        _aahText[11][0] = TEXT_CreateEx(30, 350, 144, 25,hWindow,WM_CF_SHOW,0,13,"防雷器状态:");
        _aahText[12][0] = TEXT_CreateEx(30, 380, 144, 25,hWindow,WM_CF_SHOW,0,13,"输出继电器:");
        _aahText[13][0] = TEXT_CreateEx(30, 410, 120, 25,hWindow,WM_CF_SHOW,0,13,"控制导引:");

        for(x = 0;x < ERROR_LINE;x++)
        {
            TEXT_SetFont(_aahText[x][0], &XBF24_Font);
            TEXT_SetTextColor(_aahText[x][0], GUI_BLACK);
        }
        TEXT_SetTextColor(_aahText[0][0], GUI_RED);
    }

}
// USER START (Optionally insert additional static code)
static void Timer_Process(WM_MESSAGE *pMsg)
{
    uint8_t i = 0;
    uint8_t strPowerFee[10];
    uint8_t strServiceFee[10];
    WM_HWIN hWin_Error;
    WM_HWIN hWin = pMsg->hWin;

    Caculate_RTC_Show(pMsg, ID_TEXT_1, ID_TEXT_2);

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
    sprintf(strPowerFee, "%.2lf", pEVSE->info.dDefSegFee);
    sprintf(strServiceFee, "%.2lf", pEVSE->info.dServiceFee);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), strPowerFee);//电费
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), strServiceFee);//服务费

    /// TODO (zshare#1#): 增加故障弹窗
//    if(1)//故障
//    {
//        _cbDialog_Error();//hWin_Error = GUI_CreateDialogBox(_aDialogCreate_Error, GUI_COUNTOF(_aDialogCreate_Error), _cbDialog_Error, WM_HBKWIN, 0, 0);
//    }
}
// USER END
static void _cbFrame(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      int Id = WM_GetId(pMsg->hWinSrc);      // Id of widget
        ;
    }
    break;
  }
}
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
    uint8_t *buf = "56";
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //

        Window_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4);
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_10), &XBF24_Font, GUI_RED, "欢迎使用北京动力源交流充电桩");

        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2), GUI_TA_HCENTER | GUI_TA_VCENTER,
                    &XBF24_Font, BUTTON_CI_UNPRESSED, GUI_BLUE, BUTTON_CI_UNPRESSED, GUI_RED, "          ");

//        _framebutton = FRAMEWIN_AddButton(pMsg->hWin,FRAMEWIN_BUTTON_RIGHT,0,ID_BUTTON_MANAGER);
//
//        BUTTON_GetFont();
//        BUTTON_GetText();
        //
        // Initialization of 'Image'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
        IMAGE_SetBMP(hItem, pData, FileSize);
//        dispbmp2("system/girl.bmp", 0, 5, 5, 1, 1,pMsg->hWin);
        //
        // Initialization of 'Edit'
        //
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), &XBF24_Font, " ");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), &XBF24_Font, " ");
        EDIT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), GUI_TA_RIGHT | GUI_TA_VCENTER);
        EDIT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), GUI_TA_RIGHT | GUI_TA_VCENTER);
        // Initialization of 'Text'
        //
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), &XBF36_Font, GUI_BLACK, "请选择支付方式");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5), &XBF24_Font, GUI_BLACK, "充电费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6), &XBF24_Font, GUI_BLACK, "元/度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_7), &XBF24_Font, GUI_BLACK, "服务费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_8), &XBF24_Font, GUI_BLACK, "元/度");
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), GUI_TA_LEFT | GUI_TA_VCENTER,
                    &XBF24_Font, BUTTON_CI_UNPRESSED, GUI_BLUE, BUTTON_CI_UNPRESSED, GUI_BLUE, "手机支付请扫描二维码");
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1), GUI_TA_HCENTER | GUI_TA_VCENTER,
                    &XBF24_Font, BUTTON_CI_UNPRESSED, GUI_BLUE, BUTTON_CI_UNPRESSED, GUI_BLUE, "刷卡支付请刷卡");

//        Button_Show(WM_GetDialogItem(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT), GUI_TA_HCENTER | GUI_TA_VCENTER,
//                    &XBF24_Font, BUTTON_CI_UNPRESSED, GUI_BLUE, BUTTON_CI_UNPRESSED, GUI_BLUE, "管理");

        //WM_SetCallback(pMsg->hWin, _cbFrame);
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
        Timer_Process(pMsg);
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
*       CreateWindow
*/
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void)
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

void PutOut_Window()
{
    WM_HWIN hWin;
    EventBits_t uxBitRFID;
    hWin = CreateWindow();

    while(1)
    {
        GUI_Delay(500);
        uxBitRFID = xEventGroupWaitBits(pRFIDDev->xHandleEventGroupRFID,
                                        defEventBitGotIDtoHMI,
                                        pdTRUE, pdTRUE, 0);
        if((uxBitRFID & defEventBitGotIDtoHMI) == defEventBitGotIDtoHMI)
        {
            WM_DeleteWindow(hWin);
            PutOut_Card_Info();
        }
        dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);

        vTaskDelay(500);
    }
}
// USER END

/*************************** End of file ****************************/


