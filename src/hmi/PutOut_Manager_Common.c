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
*
*   BUTTON_0    信息查询                                             *
*   BUTTON_1    历史记录
*   BUTTON_2    系统配置
*   BUTTON_3    退出
*   BUTTON_4    模拟量 告警记录
*   BUTTON_5    状态量 充电记录                                         *
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

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
// USER START (Optionally insert additional defines)
#define ID_IMAGE_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1     (GUI_ID_USER + 0x02)
#define ID_TEXT_2     (GUI_ID_USER + 0x03)
#define ID_TEXT_3     (GUI_ID_USER + 0x04)
#define ID_TEXT_4     (GUI_ID_USER + 0x05)
#define ID_MULTIPAGE_0 (GUI_ID_USER + 0x06)

#define ID_TimerTime    1
#define ID_TimerFlush   2
#define ID_TimerSignal  3
// USER END
static WM_HWIN _hWinManagerCommon;
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
	{ MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 0, 800, 320, 0, 0x0, 0 },//multipage
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
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
    int          page;
    WM_HWIN     hWinPage;

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        break;
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //
        FrameWin_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4,ID_IMAGE_0);
        //
        //初始化所有的page
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
        MULTIPAGE_SetFont(hItem, &SIF16_Font);

        hWinPage = CreateManagerInfoAnalog(pMsg->hWin);
        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "模拟量");

        hWinPage = CreateManagerInfoStatus(pMsg->hWin);
        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "状态量");

//        hWinPage = CreateManagerLogDate(pMsg->hWin);
//        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "记录查询");

//        hWinPage = CreateWindowPage4();
//        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "费率查询");

//        hWinPage = CreateManagerSysSet(pMsg->hWin);
//        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "系统配置");

        hWinPage = CreateManagerConSet(pMsg->hWin);
        MULTIPAGE_AddEmptyPage(hItem, hWinPage, "枪A配置");
        MULTIPAGE_SelectPage(hItem,0);
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_MULTIPAGE_0: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            page = MULTIPAGE_GetSelection(WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0));
            switch(page)
            {
            case 0:
                //发送对应页初始化的消息
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            }
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        }
        break;
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

            WM_RestartTimer(pMsg->Data.v,5000);
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
 *  输出管理员界面里的共用部分
 * @param
 * @param
 * @return
 *       CreateManagerCommon
*/
WM_HWIN CreateManagerCommon(void)
{
    _hWinManagerCommon = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    cur_win = _hWinManagerCommon;
}
/*************************** End of file ****************************/



