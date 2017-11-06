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
*
*   数据变化刷新;
*   需要刷新更新发送刷新数据消息
*   multi更新数据
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "xbffontcreate.h"
#include "touchtimer.h"
#include "SCROLLBAR.h"
// USER END

#include "DIALOG.h"
/*******************************************************************
*
*       defines
*
********************************************************************
*/
/*编辑窗口14行1列，状态项14个*/
#define _SYSEDIT_MAX_X 5
#define _SYSEDIT_MAX_Y 16
#define _SYSSTATUE_LINE 20
#define _SYSSTATUE_CAL 5
//后续将编辑和文本的滚轮方式用链表进行封装
#define _FONT_WIDTH 7
#define _WORD_WIDTH 12
static uint8_t _aahSysSet[_SYSEDIT_MAX_Y];
static EDIT_Handle   _aahEdit[_SYSEDIT_MAX_Y][_SYSEDIT_MAX_X];
static TEXT_Handle   _aahText[_SYSSTATUE_LINE][_SYSSTATUE_CAL];
static int _x,_y;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_IMAGE_0     (GUI_ID_USER + 0x1C)
// USER START (Optionally insert additional defines)
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
#define ID_TEXT_7  (GUI_ID_USER + 0x09)//
#define ID_TEXT_8  (GUI_ID_USER + 0x0A)//
#define ID_TEXT_9  (GUI_ID_USER + 0x0F)//
#define ID_TEXT_10  (GUI_ID_USER + 0x10)//
#define ID_TEXT_11  (GUI_ID_USER + 0x11)//
#define ID_EDIT_1  (GUI_ID_USER + 0x12)//
#define ID_EDIT_2  (GUI_ID_USER + 0x13)//
#define ID_EDIT_3  (GUI_ID_USER + 0x14)//
#define ID_EDIT_4  (GUI_ID_USER + 0x15)//
#define ID_EDIT_5  (GUI_ID_USER + 0x16)//
#define ID_EDIT_6  (GUI_ID_USER + 0x17)//
#define ID_MULTIEDIT_0 (GUI_ID_USER + 0x18)

#define ID_TimerTime    1
#define ID_TimerFlush   2
#define ID_TimerSignal  3
// USER END
static WM_HWIN hWindow;
static WM_HWIN _hWinManagerSysSet;
static WM_HTIMER _timerRTC,_timerData,_timerSignal;

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
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 789, 459, 0, 0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 630, 0, 80, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 720, 0, 70, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 440, 0, 180, 16, 0, 0x0, 0 },//网络信号强度
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 225, 367, 300, 20, 0, 0x0, 0 },//最底端的说明
    { BUTTON_CreateIndirect, "信息查询", ID_BUTTON_0, 50, 70, 150, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "历史记录", ID_BUTTON_1, 50, 140, 150, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "系统配置", ID_BUTTON_2, 50, 210, 150, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "退    出", ID_BUTTON_3, 50, 280, 150, 50, 0, 0x0, 0 },
};
/*******************************************************************
*
*       _cbWindow

  The callback moves the edit-fiels when a notification message
  was send.
*/
static void _cbWindow(WM_MESSAGE *pMsg) {
    WM_SCROLL_STATE ScrollState;
    WM_HWIN      hItem;
    int x, y;
    volatile int id = 0;
    volatile int v = 0;
    uint8_t _tmpBuff[50];

    switch (pMsg->MsgId)
    {
        case WM_NOTIFY_PARENT:
            /**< 添加两个滑轮的事件 */
            switch(WM_GetId(pMsg->hWinSrc))
            {
                case GUI_ID_HSCROLL://水平
                    if(pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED)
                    {
                        /* 得到滚动条的状态，得到的数值好像是负值 才能使得 _x - ScrollState.v是正值 */
                        WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
                        if (_x != ScrollState.v)
                        {
                            for (y = 0; y < _SYSEDIT_MAX_Y; y++)
                            {
                                for (x = 0; x < _SYSEDIT_MAX_X; x++)
                                {
                                    WM_MoveWindow(_aahEdit[y][x], _x - ScrollState.v, 0);
                                    WM_MoveWindow(_aahText[y][x], _x - ScrollState.v, 0);
                                }
                            }
                            _x = ScrollState.v;
                        }
                    }
                break;
                case GUI_ID_VSCROLL:
                    if(pMsg->Data.v == WM_NOTIFICATION_VALUE_CHANGED)
                    {
                        WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
                        if (_y != ScrollState.v)
                        {
                            for (y = 0; y < _SYSEDIT_MAX_Y; y++)
                            {
                                for (x = 0; x < _SYSEDIT_MAX_X; x++)
                                {
                                    WM_MoveWindow(_aahEdit[y][x],0, _y - ScrollState.v);
                                    WM_MoveWindow(_aahText[y][x],0, _y - ScrollState.v);
                                }
                            }
                            _y = ScrollState.v;
                        }
                    }
                break;
                case 20:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        //WM_SendMessageNoPara(hWin,MSG_JUMPKEYPAD);
//                        WM_SetStayOnTop(hWindow,0);
//                        GUI_EndDialog(hWindow,0);
//                       _deleteWin(_hWinManagerSysSet);
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,20,hWindow,_hWinManagerSysSet,"交流桩序列号","eg,1122334455667788");
                        //Keypad_GetValue(SYSSET_VALUE,"交流桩序列号");
                        EDIT_SetText(_aahEdit[0][0],pEVSE->info.strSN);
                    }
                break;
                case 21:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,21,hWindow,_hWinManagerSysSet,"充电枪个数","eg,1");
                        memset(_tmpBuff,'\0',sizeof(_tmpBuff));
                        sprintf(_tmpBuff,"%d",pEVSE->info.ucTotalCON);
                        EDIT_SetText(_aahEdit[1][0],_tmpBuff);
                    }
                break;
                case 22:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,22,hWindow,_hWinManagerSysSet,"交流电压范围下限","eg,200");
                        memset(_tmpBuff,'\0',sizeof(_tmpBuff));
                        sprintf(_tmpBuff,"%.1f",pCon->info.dVolatageLowerLimits);
                        EDIT_SetText(_aahEdit[2][0],_tmpBuff);
                    }
                break;
                case 23:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,23,hWindow,_hWinManagerSysSet,"交流电压范围上限","eg,260");
                        memset(_tmpBuff,'\0',sizeof(_tmpBuff));
                        sprintf(_tmpBuff,"%.1f",pCon->info.dVolatageUpperLimits);
                        EDIT_SetText(_aahEdit[2][1],_tmpBuff);
                    }
                break;
                case 24:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,24,hWindow,_hWinManagerSysSet,"交流电流范围下限","eg,4.0");
                    }
                break;
                case 25:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,25,hWindow,_hWinManagerSysSet,"交流电流范围上限","eg,60");
                    }
                break;
                case 26:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,26,hWindow,_hWinManagerSysSet,"过温告警值","eg,80");
                        memset(_tmpBuff,'\0',sizeof(_tmpBuff));
                        sprintf(_tmpBuff,"%.1f",pCon->info.dACTempUpperLimits);
                        EDIT_SetText(_aahEdit[4][0],_tmpBuff);
                    }
                break;
                case 27:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,27,hWindow,_hWinManagerSysSet,"屏保时间","eg,60");
                    }
                break;
                case 28:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,28,hWindow,_hWinManagerSysSet,"电表地址","eg,1");
                    }
                break;
                case 29:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,29,hWindow,_hWinManagerSysSet,"电表波特率","eg,115200");
                    }
                break;
                case 30:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,30,hWindow,_hWinManagerSysSet,"本机IP","eg,192.168.1.14");
                    }
                break;
                case 31:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,31,hWindow,_hWinManagerSysSet,"子网掩码","eg,255.255.255.0");
                    }
                break;
                case 32:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,32,hWindow,_hWinManagerSysSet,"网关","eg,192.168.1.1");
                    }
                break;
                case 33:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,33,hWindow,_hWinManagerSysSet,"MAC地址","eg,01:8a:03:9b:05:09");
                    }
                break;
                case 34:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,34,hWindow,_hWinManagerSysSet,"服务器端口","eg,8080");
                    }
                break;
                case 35:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,35,hWindow,_hWinManagerSysSet,"获取SSID","eg,dpc");
                    }
                break;
                case 36:
                    if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
                    {
                        WM_HideWindow(hWindow);
                        WM_HideWindow(_hWinManagerSysSet);

                        Keypad_GetValueTest(SYSSET_VALUE,36,hWindow,_hWinManagerSysSet,"设置密码","eg,dpc123");
                    }
                break;

            }
            break;
        case WM_PAINT:
            /**< 绘制滑轮围绕区颜色 */
            GUI_SetBkColor(GUI_WHITE);
            GUI_Clear();
            break;
        default:
            WM_DefaultProc(pMsg);
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
    int x, y;
    SCROLLBAR_Handle hScroll;
    SCROLLBAR_Handle wScroll;
    char _tmpBuff[50];
    CON_t *pCon;

    pCon = CONGetHandle(0);
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        //LCD_SetCursor(0,200);
        WM_SetFocus(pMsg->hWin);
        /// TODO (zshare#1#): 下面的if不起作用.\
        但是if里嵌套的if起作用,目前先用此来规避不起作用的if
        if(_hWinManagerSysSet == cur_win)
        {
            /**< 数据处理 */
            //Data_Process(pMsg);
            /**< 信号数据处理 */
            Signal_Show();
            /**< 灯光控制 */
            Led_Show();
            /**< 如果界面发生了切换 */
            if(_hWinManagerSysSet == cur_win)
            {
                /**< 故障分析 */
                /// TODO (zshare#1#): 滑轮页均不对故障处理.故障界面被滑轮页覆盖
                //Err_Analy(pMsg->hWin);
                /**< 特殊触控点分析 */
                CaliDone_Analy(pMsg->hWin);
            }
        }
        break;
    case WM_INIT_DIALOG:
        _x = 0;
        _y = 0;

        //
        // Initialization of 'Framewin'
        //
        FrameWin_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4,ID_IMAGE_0);

        // 创建窗口250
        hWindow = WM_CreateWindow(230, 70, 520, 350, WM_CF_SHOW, &_cbWindow, 0);
        //创建水平滑轮
        hScroll = SCROLLBAR_CreateAttached(hWindow, 0);//水平滑轮
        //设置滑轮条目数量
        SCROLLBAR_SetNumItems(hScroll, _SYSEDIT_MAX_X);
        //设置页尺寸
        //SCROLLBAR_SetPageSize(hScroll, 220);
        SCROLLBAR_SetWidth(hScroll,20);
        //创建垂直滑轮
        wScroll = SCROLLBAR_CreateAttached(hWindow, SCROLLBAR_CF_VERTICAL);//垂直滑轮
        //设置滑轮条目数量
        SCROLLBAR_SetNumItems(wScroll, 25*10);
        //设置页尺寸
        //SCROLLBAR_SetPageSize(wScroll, 220);
        SCROLLBAR_SetWidth(wScroll,20);

        //创建文本区 -- 24号字体 4-96 5-120 6-144 7-168 8-192
        /**< 13文本固定id */
        //创建编辑区
        /**< 20-50编辑区ID */
        _aahText[0][0] = TEXT_CreateEx(30, 20, _FONT_WIDTH*(strlen("交流桩序列号:")), 25,hWindow,WM_CF_SHOW,0,13,"交流桩序列号:");
        _aahEdit[0][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("交流桩序列号:")), 20, _WORD_WIDTH*(strlen("1122334455667788")), 25,hWindow,WM_CF_SHOW,0,20,strlen("1122334455667788"));
        EDIT_SetText(_aahEdit[0][0],pEVSE->info.strSN);

        _aahText[1][0] = TEXT_CreateEx(30, 50, _FONT_WIDTH*(strlen("充电枪个数:")), 25,hWindow,WM_CF_SHOW,0,13,"充电枪个数:");
        _aahEdit[1][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("充电枪个数:")), 50, _WORD_WIDTH*(strlen("255")) , 25,hWindow,WM_CF_SHOW,0,21,strlen("255"));
        _aahText[1][1] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("充电枪个数:"))+_WORD_WIDTH*(strlen("255")), 50, _WORD_WIDTH*strlen("个"), 25,hWindow,WM_CF_SHOW,0,13,"个");
        memset(_tmpBuff,'\0',strlen(_tmpBuff));
        sprintf(_tmpBuff,"%d",pEVSE->info.ucTotalCON);
        printf_safe("pEVSE->info.ucTotalCON = %s\n",_tmpBuff);
        EDIT_SetText(_aahEdit[1][0],_tmpBuff);

        _aahText[2][0] = TEXT_CreateEx(30, 80, _FONT_WIDTH*(strlen("交流电压范围:")), 25,hWindow,WM_CF_SHOW,0,13,"交流电压范围:");
        _aahEdit[2][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("交流电压范围:")),80, _WORD_WIDTH*(strlen("255.5")), 25,hWindow,WM_CF_SHOW,0,22,strlen("255.5"));
        _aahText[2][1] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("交流电压范围:"))+_WORD_WIDTH*(strlen("255.5")), 80, _FONT_WIDTH*(strlen(" ~ ")), 25,hWindow,WM_CF_SHOW,0,13," ~ ");
        _aahEdit[2][1] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("交流电压范围:"))+_WORD_WIDTH*(strlen("255.5"))+_FONT_WIDTH*(strlen(" ~  ")),80, _WORD_WIDTH*(strlen("255.5")), 25,hWindow,WM_CF_SHOW,0,23,strlen("255.5"));
        _aahText[2][2] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("交流电压范围:"))+_WORD_WIDTH*(strlen("255.5"))+_FONT_WIDTH*(strlen(" ~ "))+_WORD_WIDTH*strlen("255.5"),80,_WORD_WIDTH*(strlen(" V")),25,hWindow,WM_CF_SHOW,0,13," V");
        memset(_tmpBuff,'\0',strlen(_tmpBuff));
        sprintf(_tmpBuff,"%.1f",pCon->info.dVolatageLowerLimits);
        printf_safe("pCon->info.dVolatageLowerLimits = %s\n",_tmpBuff);
        EDIT_SetText(_aahEdit[2][0],_tmpBuff);
        sprintf(_tmpBuff,"%.1f",pCon->info.dVolatageUpperLimits);
        printf_safe("pCon->info.dVolatageUpperLimits = %s\n",_tmpBuff);
        EDIT_SetText(_aahEdit[2][1],_tmpBuff);

        _aahText[3][0] = TEXT_CreateEx(30, 110, _FONT_WIDTH*(strlen("交流电流范围:")), 25,hWindow,WM_CF_SHOW,0,13,"交流电流范围:");
        _aahEdit[3][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("交流电流范围:")),110, _WORD_WIDTH*(strlen("255.5")), 25,hWindow,WM_CF_SHOW,0,24,strlen("255.5"));
        _aahText[3][1] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("交流电流范围:"))+_WORD_WIDTH*(strlen("255.5")), 110, _FONT_WIDTH*(strlen(" ~ ")), 25,hWindow,WM_CF_SHOW,0,13," ~ ");
        _aahEdit[3][1] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("交流电流范围:"))+_WORD_WIDTH*(strlen("255.5"))+_FONT_WIDTH*(strlen(" ~  ")),110, _WORD_WIDTH*(strlen("255.5")), 25,hWindow,WM_CF_SHOW,0,25,strlen("255.5"));
        _aahText[3][2] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("交流电流范围:"))+_WORD_WIDTH*(strlen("255.5"))+_FONT_WIDTH*(strlen(" ~ "))+_WORD_WIDTH*strlen("255.5"),110,_WORD_WIDTH*(strlen(" A")),25,hWindow,WM_CF_SHOW,0,13," A");
        EDIT_SetText(_aahEdit[3][0],"12.5");
        EDIT_SetText(_aahEdit[3][1],"45.5");

        _aahText[4][0] = TEXT_CreateEx(30, 140, _FONT_WIDTH*(strlen("过温告警值:")), 25,hWindow,WM_CF_SHOW,0,13,"过温告警值:");
        _aahEdit[4][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("过温告警值:")), 140, _WORD_WIDTH*(strlen("145.4")), 25,hWindow,WM_CF_SHOW,0,26,strlen("145.4"));
        _aahText[4][1] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("过温告警值:"))+_WORD_WIDTH*strlen("145.4"), 140, _WORD_WIDTH*(strlen(" ℃")), 25,hWindow,WM_CF_SHOW,0,13,"℃");
        sprintf(_tmpBuff,"%.1f",pCon->info.dACTempUpperLimits);
        EDIT_SetText(_aahEdit[4][0],_tmpBuff);

        _aahText[5][0] = TEXT_CreateEx(30, 170, _FONT_WIDTH*(strlen("屏保时间:")), 25,hWindow,WM_CF_SHOW,0,13,"屏保时间:");
        _aahEdit[5][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("屏保时间:")), 170, _WORD_WIDTH*(strlen("100")), 25,hWindow,WM_CF_SHOW,0,27,strlen("100"));
        _aahText[5][1] = TEXT_CreateEx(30+_FONT_WIDTH*(strlen("屏保时间:"))+_WORD_WIDTH*(strlen("100")), 170, _FONT_WIDTH*(strlen("分")), 25,hWindow,WM_CF_SHOW,0,13,"分");
        EDIT_SetText(_aahEdit[5][0],"60");

        _aahText[6][0] = TEXT_CreateEx(30, 200, _FONT_WIDTH*(strlen("电表地址:")), 25,hWindow,WM_CF_SHOW,0,13,"电表地址:");
        _aahEdit[6][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("电表地址:")), 200, _WORD_WIDTH*(strlen("255")), 25,hWindow,WM_CF_SHOW,0,28,strlen("255"));
        EDIT_SetText(_aahEdit[6][0],"255");

        _aahText[7][0] = TEXT_CreateEx(30, 230, _FONT_WIDTH*(strlen("电表波特率:")), 25,hWindow,WM_CF_SHOW,0,13,"电表波特率:");
        _aahEdit[7][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("电表波特率:")),230, _WORD_WIDTH*(strlen("115200")), 25,hWindow,WM_CF_SHOW,0,29,strlen("115200"));
        EDIT_SetText(_aahEdit[7][0],"115200");

        _aahText[8][0] = TEXT_CreateEx(30, 260, _FONT_WIDTH*(strlen("本机IP:")), 25,hWindow,WM_CF_SHOW,0,13,"本机IP:");
        _aahEdit[8][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("本机IP: ")),260, _WORD_WIDTH*(strlen("255.255.255.255")), 25,hWindow,WM_CF_SHOW,0,30,strlen("255.255.255.255"));
        EDIT_SetText(_aahEdit[8][0],"255.255.255.255");

        _aahText[9][0] = TEXT_CreateEx(30, 290, _FONT_WIDTH*(strlen("子网掩码:")), 25,hWindow,WM_CF_SHOW,0,13,"子网掩码:");
        _aahEdit[9][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("子网掩码:")), 290, _WORD_WIDTH*(strlen("255.255.255.255")), 25,hWindow,WM_CF_SHOW,0,31,strlen("255.255.255.255"));
        EDIT_SetText(_aahEdit[9][0],"255.255.255.255");

        _aahText[10][0] = TEXT_CreateEx(30, 320, _FONT_WIDTH*(strlen("网关:")), 25,hWindow,WM_CF_SHOW,0,13,"网关:");
        _aahEdit[10][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("网关:")),320, _WORD_WIDTH*(strlen("255.255.255.255")), 25,hWindow,WM_CF_SHOW,0,32,strlen("255.255.255.255"));
        EDIT_SetText(_aahEdit[10][0],"255.255.255.255");

        _aahText[11][0] = TEXT_CreateEx(30, 350, _FONT_WIDTH*(strlen("MAC地址:")), 25,hWindow,WM_CF_SHOW,0,13,"MAC地址:");
        _aahEdit[11][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("MAC地址: ")),350, _WORD_WIDTH*strlen("00:00:00:00:00:00"), 25,hWindow,WM_CF_SHOW,0,33,strlen("00:00:00:00:00:00"));
        EDIT_SetText(_aahEdit[11][0],"00:00:00:00:00:00");

        _aahText[12][0] = TEXT_CreateEx(30, 380, _FONT_WIDTH*(strlen("服务器端口:")), 25,hWindow,WM_CF_SHOW,0,13,"服务器端口:");
        _aahEdit[12][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("服务器端口:")), 380, _WORD_WIDTH*strlen("8080"), 25,hWindow,WM_CF_SHOW,0,34,strlen("8080"));
        EDIT_SetText(_aahEdit[12][0],"8080");

        _aahText[13][0] = TEXT_CreateEx(30, 410, _FONT_WIDTH*(strlen("获取SSID:")), 25,hWindow,WM_CF_SHOW,0,13,"获取SSID:");
        _aahEdit[13][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("获取SSID:")),410, _WORD_WIDTH*strlen("woshinidaye"), 25,hWindow,WM_CF_SHOW,0,35,strlen("woshinidaye"));
        EDIT_SetText(_aahEdit[13][0],"dpc");

        _aahText[14][0] = TEXT_CreateEx(30, 440, _FONT_WIDTH*(strlen("设置密码:")), 25,hWindow,WM_CF_SHOW,0,13,"设置密码:");
        _aahEdit[14][0] = EDIT_CreateEx(30+_FONT_WIDTH*(strlen("设置密码: ")),440, _WORD_WIDTH*strlen("woshinidaye"), 25,hWindow,WM_CF_SHOW,0,36,strlen("woshinidaye"));
        EDIT_SetText(_aahEdit[14][0],"1234567890");

        for(x = 0;x < _SYSSTATUE_LINE;x++)
        {
            for(y = 0;y < _SYSSTATUE_CAL;y++)
            {
                TEXT_SetFont(_aahText[x][y], &SIF16_Font);
                TEXT_SetTextColor(_aahText[x][y], GUI_BLACK);
            }
        }

        for (y = 0; y < _SYSEDIT_MAX_Y; y++)
        {
            for (x = 0; x < _SYSEDIT_MAX_X; x++)
            {
                EDIT_SetTextAlign(_aahEdit[y][x], GUI_TA_HCENTER | GUI_TA_VCENTER);
                EDIT_SetFont(_aahEdit[y][x], &SIF16_Font);
            }
        }

        WM_SetStayOnTop(hWindow,1);
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_BLACK,BUTTON_CI_UNPRESSED,GUI_BLACK,"信息查询");

        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_BLACK,BUTTON_CI_UNPRESSED,GUI_BLACK,"历史查询");

        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_RED,BUTTON_CI_UNPRESSED,GUI_RED,"系统配置");
        BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),1);
        BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),BUTTON_CI_PRESSED,GUI_RED);

        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_BLACK,BUTTON_CI_UNPRESSED,GUI_BLACK,"退    出");
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;

        switch(Id) {
        case ID_BUTTON_0: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                /**< 跳转到信息查询 */
                WM_SetStayOnTop(hWindow,0);
                GUI_EndDialog(hWindow,0);
                _deleteWin(_hWinManagerSysSet);
                CreateManagerInfoAnalog();
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
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                /**< 跳转到历史记录查询 */
                WM_SetStayOnTop(hWindow,0);
                GUI_EndDialog(hWindow,0);
                _deleteWin(_hWinManagerSysSet);
                CreateManagerAlarmLog();
            break;
          case WM_NOTIFICATION_RELEASED:

            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_BUTTON_2: // Notifications sent by 'Button'
          switch(NCode) {
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
        case ID_BUTTON_3: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            /**< 跳转至home */
            WM_SetStayOnTop(hWindow,0);
            GUI_EndDialog(hWindow,0);
            _deleteWin(_hWinManagerSysSet);
            CreateHome();
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        }
        break;
        // USER START (Optionally insert additional message handling)
    case WM_TIMER:
        if(pMsg->Data.v == _timerRTC)
        {
            /**< 显示时间和日期 */
            Caculate_RTC_Show(pMsg, ID_TEXT_1, ID_TEXT_2);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_3), strCSQ);
            /**< 重启定时器 */
            WM_RestartTimer(pMsg->Data.v, 20);
        }
        if(pMsg->Data.v == _timerSignal)
        {
            WM_RestartTimer(pMsg->Data.v, 2000);
        }
        if(pMsg->Data.v == _timerData)
        {
            //Data_Flush(pMsg);
            //Status_Content_Analy();
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
 *  输出管理员界面里的系统配置
 * @param
 * @param
 * @return
 *
 *
 *       CreateManagerSysSet
*/
WM_HWIN CreateManagerSysSet(void);
WM_HWIN CreateManagerSysSet(void)
{
//    CON_t *pCon;
//    pCon = CONGetHandle(0);
//    SetCONCfg(pCon,"SocketType",'B',ParamTypeU8);
//    SetCONCfg(pEVSE->info.dDefSegFee,"DefSegFee","123456",ParamTypeString);

    _hWinManagerSysSet = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    cur_win = _hWinManagerSysSet;
    _timerRTC = WM_CreateTimer(WM_GetClientWindow(_hWinManagerSysSet), ID_TimerTime, 20, 0);
    _timerData = WM_CreateTimer(WM_GetClientWindow(_hWinManagerSysSet), ID_TimerFlush,1000,0);
    _timerSignal = WM_CreateTimer(WM_GetClientWindow(_hWinManagerSysSet), ID_TimerSignal,5000,0);
}
/*************************** End of file ****************************/




