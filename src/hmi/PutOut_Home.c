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
#include "xbffontcreate.h"
#include "HMI_Start.h"
#include "touchtimer.h"
#include "bmpdisplay.h"
#include "touchtimer.h"
#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_WINDOW_0     (GUI_ID_USER + 0x10)
#define ID_BUTTON_MANAGER   (GUI_ID_USER + 0x12)
#define ID_TEXT_0     (GUI_ID_USER + 0x11)
#define ID_IMAGE_0     (GUI_ID_USER + 0x0A)
#define ID_IMAGE_1      (GUI_ID_USER + 0x14)
#define ID_IMAGE_0_IMAGE_0     0x00

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
#define ID_TEXT_A     (GUI_ID_USER + 0x01)
#define ID_TEXT_B     (GUI_ID_USER + 0x04)
#define ID_MULTIEDIT_0  (GUI_ID_USER + 0x13)
#define ID_TimerTime    0
WM_HWIN cur_win;
WM_HWIN _hWinHome;
uint8_t strCSQ[10];
extern uint8_t *bmpbuffer;
extern FIL BMPFile_ENCODE;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aDialogCreate
* 主窗口的结构体描述
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 789, 459, 0, 0, 0 },//尝试bmp单独显示
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_1, 130, 170, 150, 150, 0, 0, 0 },//二维码显示
    { TEXT_CreateIndirect, "Text", ID_TEXT_A, 67, 80, 250, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_B, 450, 80, 250, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 630, 0, 80, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 720, 0, 70, 16, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 440, 0, 180, 16, 0, 0x0, 0 },//网络信号强度
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 225, 367, 300, 20, 0, 0x0, 0 },//最底端的说明
    { TEXT_CreateIndirect, "Text", ID_TEXT_5, 422, 177, 80, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 510, 177, 80, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_6, 598, 177, 80, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_7, 422, 216, 80, 30, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 510, 216, 80, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_8, 598, 216, 80, 30, 0, 0x0, 0 },
};
/*********************************************************************
*
*       Static code
*   数据处理函数
**********************************************************************
*/
static void Data_Process(WM_MESSAGE *pMsg)
{
    CON_t *pCON;
    uint8_t strPowerFee[10];
    uint8_t strServiceFee[10];
    WM_HWIN hWin_Error;
    EventBits_t uxBitRFID;
    EventBits_t uxBits;

    WM_HWIN hWin = pMsg->hWin;

    pCON = CONGetHandle(0);
    /**< 刷卡充电跳页 */
    uxBitRFID = xEventGroupWaitBits(pRFIDDev->xHandleEventGroupRFID,
                                    defEventBitGotIDtoHMI,
                                    pdTRUE, pdTRUE, 0);
    if((uxBitRFID & defEventBitGotIDtoHMI) == defEventBitGotIDtoHMI)
    {
//        GUI_EndDialog(_hWinHome,0);
//        _hWinHome = 0;
//        CreateCardInfo();
        WM_SendMessageNoPara(hWin,MSG_JUMPCARDINFO);
    }
    /**< 扫码充电跳页 */
    if((pCON->order.ucStartType == 5)
        &&(pCON->state == STATE_CON_CHARGING))
    {
        WM_SendMessageNoPara(hWin,MSG_JUMPCHAING);
    }

    if(pCON->state == STATE_CON_CHARGING)
    {
        WM_SendMessageNoPara(hWin,MSG_JUMPCHAING);
    }

    /**< 充电费和服务费的费用值显示 */
    sprintf(strPowerFee, "%.2lf", pEVSE->info.dDefSegFee);
    sprintf(strServiceFee, "%.2lf", pEVSE->info.dServiceFee);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), strPowerFee);/**< 电费*/
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), strServiceFee);/**< 服务费 */
}
void PaintQR(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    GUI_SetColor(0x000000);
    GUI_FillRect(125, 59, 274, 68);
    GUI_SetColor(0x0000ff);
    GUI_FillRect(321, 59, 335, 208);
    GUI_QR_Draw(qr_hmem,70,170);
    GUI_DrawLine(100,100,100,200);
}
/** @brief
 *  _cbDialog 创建主窗口
 * @param pMsg:消息体
 * @param
 * @return
 *
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    const void *pData;
    WM_HWIN      hItem;
    U32          FileSize;
    int          NCode;
    int         Id;


    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        /// TODO (zshare#1#): 下面的if不起作用. 但是if里嵌套的if起作用,目前先用此来规避不起作用的if
        if((bittest(winInitDone,0))&&(_hWinHome == cur_win))
        {
            //dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
            /**< 数据刷新 */
            Data_Process(pMsg);
            /**< 信号 */
            Signal_Show();
            /**< 灯光控制 */
            Led_Show();
            /**< 如果界面发生了切换 */
            if(_hWinHome == cur_win)
            {
                /**< 故障分析 */
                Err_Analy(pMsg->hWin);
                /**< 特殊触控点分析 */
                CaliDone_Analy(pMsg->hWin);
            }
            PaintQR(pMsg);
        }
        break;
    case WM_INIT_DIALOG:
        /**< 创建framewin */
        FrameWin_Init(pMsg, ID_TEXT_1, ID_TEXT_2, ID_TEXT_3, ID_TEXT_4,ID_IMAGE_0);
        /**< text和edit的初始化 */
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), &SIF16_Font, " ");
        Edit_Show(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), &SIF24_Font, " ");
        EDIT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), GUI_TA_RIGHT | GUI_TA_VCENTER);
        EDIT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), GUI_TA_RIGHT | GUI_TA_VCENTER);

        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5), &SIF24_Font, GUI_BLACK, "充电费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6), &SIF24_Font, GUI_BLACK, "元/度");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_7), &SIF24_Font, GUI_BLACK, "服务费");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_8), &SIF24_Font, GUI_BLACK, "元/度");

        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_A), &SIF24_Font, GUI_BLACK, "手机支付请扫描二维码");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_B), &SIF24_Font, GUI_BLACK, "刷卡支付请刷卡");
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id)
        {
            /**< 跳转管理员界面的密码输入页 */
        case ID_BUTTON_MANAGER:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                //WM_DeleteWindow(pMsg->hWin);
                _deleteWin(pMsg->hWin);
                Keypad_GetValue(LOGIN_PASSWD," ");
                break;
            case WM_NOTIFICATION_RELEASED:
                //WM_DeleteWindow(pMsg->hWin);
                _deleteWin(pMsg->hWin);
                Keypad_GetValue(LOGIN_PASSWD," ");
                break;
            }
            break;
        }
        break;
    case WM_TIMER:
        /**< 显示时间和日期 */
        Caculate_RTC_Show(pMsg, ID_TEXT_1, ID_TEXT_2);
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_3), strCSQ);
        /**< 重启定时器 */
        WM_RestartTimer(pMsg->Data.v, 20);
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
            //WM_DeleteWindow(err_hItem);
        }
        break;
    case MSG_JUMPCARDINFO:
//        GUI_EndDialog(_hWinHome,0);
        _deleteWin(_hWinHome);
        //_hWinHome = 0;
        //WM_DeleteWindow(_hWinHome);
        CreateCardInfo();
        current_page = _CARDINFOPAGE;
        bitset(winInitDone,7);
        break;
    case MSG_JUMPCHAING:
//        GUI_EndDialog(_hWinHome,0);
//        _hWinHome = 0;
        //WM_DeleteWindow(_hWinHome);
        //PutOut_Charging();
        _deleteWin(_hWinHome);
        CreateCharging();
        current_page = _CHARGINGPAGE;
        bitset(winInitDone,7);
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/** @brief
 *  输出充电桩欢迎界面
 * @param
 * @param
 * @return
 *
 */
WM_HWIN CreateHome(void);
WM_HWIN CreateHome(void)
{
    _hWinHome = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    cur_win = _hWinHome;
    WM_CreateTimer(WM_GetClientWindow(_hWinHome), ID_TimerTime, 20, 0);
    //dispbmp("system/dpc.bmp", 0, 5, 5, 1, 1);
	bitset(winInitDone,0);
    return 0;
}
/*************************** End of file ****************************/

