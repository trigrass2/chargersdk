#include "touchtimer.h"

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

#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
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
//6 12 10
//1 2 1 2 1 2
//static uint8_t _alarmLog[] = {
//    "序号      开始时间            结束时间          告警内容\n\
// 01  2017-06-07 12:13:34  2017-06-08 12:13:23  急停故障"
//};
WM_HWIN _hWinManagerLogDate;
static WM_HWIN _hWinManagerLog;
static WM_HTIMER _timerRTC,_timerData,_timerSignal;
uint16_t column_num,row_num;

static const char *_apYear[]=
{
    "2017", "2018", "2019", "2020", "2021",
    "2022", "2023", "2024", "2025", "2026",
    "2027", "2028", "2029", "2030", "2031",
    "2032", "2033", "2034", "2035", "2036",
    "2037", "2038", "2039", "2040", "2041",
    "2042", "2043", "2044",
};
static const char *_apMonth[]=
{
    "01","02","03","04","05","06",
    "07","08","09","10","11","12",
};
static const char *_apDay[]=
{
    "01", "02", "03", "04", "05", "06",
    "07", "08", "09", "10", "11", "12",
    "13", "14", "15", "16", "17", "18",
    "19", "20", "21", "22", "23", "24",
    "25", "26", "27", "28", "29", "30", "31",
};
typedef struct
{
    int tm_year;
    int tm_mon;
    int tm_mday;
}_MANAGERDate;
static DateStruct sel_start_date = {"2017","01","01"};
static DateStruct sel_end_date = {"2019","01","01"};
static uint8_t list_start_index[3];
static uint8_t list_end_index[3];

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
#define LISTWHEEL_XLEFTOFF  80
    { WINDOW_CreateIndirect, "Framewin", ID_WINDOW_0, 0, 20, 800, 300, 0, 0, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 20, 40, 750, 250, 0, 0x0, 0 },//560,276

    { TEXT_CreateIndirect, "起始", ID_TEXT_5, 100 - LISTWHEEL_XLEFTOFF, 140, 70, 40, 0, 0x0, 0 },

    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 190 - LISTWHEEL_XLEFTOFF, 60, 90, 180, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_1, 280 - LISTWHEEL_XLEFTOFF, 60, 50, 180, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_2, 330 - LISTWHEEL_XLEFTOFF, 60, 50, 180, 0, 0x0, 0 },

    { TEXT_CreateIndirect, "终止", ID_TEXT_6, 400 - LISTWHEEL_XLEFTOFF, 140, 70, 40, 0, 0x0, 0 },

    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_3, 490 - LISTWHEEL_XLEFTOFF, 60, 90, 180, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_4, 580 - LISTWHEEL_XLEFTOFF, 60, 50, 180, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_5, 630 - LISTWHEEL_XLEFTOFF, 60, 50, 180, 0, 0x0, 0 },

    { BUTTON_CreateIndirect, "查询时间", ID_BUTTON_3, 20, 20, 80, 20, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "查询告警", ID_BUTTON_4, 700 - LISTWHEEL_XLEFTOFF, 100, 160, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "查询充电", ID_BUTTON_5, 700 - LISTWHEEL_XLEFTOFF, 170, 160, 40, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreateList[] =
{
//    { WINDOW_CreateIndirect, "Framewin", ID_WINDOW_0, 0, 20, 800, 300, 0, 0x64, 0 },
//    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 20, 800, 276, 0, 0x0, 0 },//560,276
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
int  Data_Flush(uint8_t log_type,WM_HWIN hItem)
{
    cJSON *jsParent;
    cJSON *jsChild;
    cJSON *jsItem;
    cJSON *jsItemTmp;
    ErrorCode_t errcode;
    uint32_t ulMaxItem;
    int i = 0,j = 0;
	struct tm *ts;
	char buf[80] = "\0";
    _MANAGERDate ts_start, ts_end;

	if(0 == log_type)   //故障记录
    {
        jsParent = GetCfgObj(pathEVSELog, &errcode);
        if (jsParent == NULL)
        {
            cJSON_Delete(jsParent);
            return errcode;
        }
        ulMaxItem  = cJSON_GetArraySize(jsParent);
        if (ulMaxItem == 0)
        {
            cJSON_Delete(jsParent);
            LISTVIEW_AddRow(hItem, NULL);
            LISTVIEW_SetItemText(hItem, 0, 0, "没有记录");
            return 0;
        }

        ts_start.tm_year = (sel_start_date.year[0]-48)*1000+
                            (sel_start_date.year[1]-48)*100+
                            (sel_start_date.year[2]-48)*10+
                            (sel_start_date.year[3]-48);
        ts_start.tm_mon = (sel_start_date.month[0]-48)*10+
            (sel_start_date.month[1]-48);
        ts_start.tm_mday = (sel_start_date.day[0]-48)*10+
            (sel_start_date.day[1]-48);

        ts_end.tm_year = (sel_end_date.year[0] - 48) * 1000 +
            (sel_end_date.year[1]-48) * 100 +
            (sel_end_date.year[2]-48) * 10 +
            (sel_end_date.year[3]-48);
        ts_end.tm_mon = (sel_end_date.month[0]-48) * 10 +
            (sel_end_date.month[1]-48);
        ts_end.tm_mday = (sel_end_date.day[0]-48) * 10 +
            (sel_end_date.day[1]-48);

        for (j = 0; j < ulMaxItem; j++)
        {
            jsChild = cJSON_GetArrayItem(jsParent, ulMaxItem - j - 1);
            jsItem = cJSON_GetObjectItem(jsChild, jnLogTime);
	        ts = localtime((time_t*)&(jsItem->valueint));

            //判断起始年
            if((ts->tm_year+1900) < ts_start.tm_year)
            {
                continue;
            }
            else if((ts->tm_year+1900) == ts_start.tm_year)
            {
                if((ts->tm_mon+1) < ts_start.tm_mon)
                {
                    continue;
                }
                else if((ts->tm_mon+1) == ts_start.tm_mon)
                {
                    if(ts->tm_mday < ts_start.tm_mday)
                    {
                        continue;
                    }
                }
            }
            //判断截止年
            if((ts->tm_year+1900) > ts_end.tm_year)
            {
                continue;
            }
            else if((ts->tm_year+1900) == ts_end.tm_year)
            {
                if((ts->tm_mon+1) > ts_end.tm_mon)
                {
                    continue;
                }
                else if((ts->tm_mon+1) == ts_end.tm_mon)
                {
                    if(ts->tm_mday > ts_end.tm_mday)
                    {
                        continue;
                    }
                }
            }

            //序号 记录时间  枪号  故障等级  故障状态  故障信息
            LISTVIEW_AddRow(hItem, NULL);

            sprintf((char *)buf, "%d", i+1);
            LISTVIEW_SetItemText(hItem, 0, i, buf);

//            jsChild = cJSON_GetArrayItem(jsParent, ulMaxItem - i - 1);

            jsItem = cJSON_GetObjectItem(jsChild, jnLogTime);
	        ts = localtime((time_t*)&(jsItem->valueint));
	        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
            LISTVIEW_SetItemText(hItem, 1, i, buf);

            jsItem = cJSON_GetObjectItem(jsChild, jnLogDevice);
            sprintf((char *)buf, "%d", jsItem->valueint);
            if (jsItem->valueint == 0)
            {
                LISTVIEW_SetItemText(hItem, 2, i, "桩");
            }
            else if (jsItem->valueint == 1)
            {
                LISTVIEW_SetItemText(hItem, 2, i, "枪");
            }

            //0 状态 1 告警 2 异常 3 故障
            jsItem = cJSON_GetObjectItem(jsChild, jnLogLevel);
            switch (jsItem->valueint)
            {
                case 0:
                    LISTVIEW_SetItemText(hItem, 3, i, "状态");
                    break;
                case 1:
                    LISTVIEW_SetItemText(hItem, 3, i, "警告");
                    break;
                case 2:
                    LISTVIEW_SetItemText(hItem, 3, i, "严重警告");
                    break;
                case 3:
                    LISTVIEW_SetItemText(hItem, 3, i, "故障");
                    break;
            }

            jsItem = cJSON_GetObjectItem(jsChild, jnLogState);
            switch (jsItem->valueint)
            {
                case 0:
                    LISTVIEW_SetItemText(hItem, 4, i, "消除");
                    break;
                case 1:
                    LISTVIEW_SetItemText(hItem, 4, i, "发生");
                    break;
            }

            jsItem = cJSON_GetObjectItem(jsChild, jnLogMessage);
            LISTVIEW_SetItemText(hItem, 5, i, jsItem->valuestring);
            i++;
        }
        if (i == 0)
        {
            LISTVIEW_AddRow(hItem, NULL);
            LISTVIEW_SetItemText(hItem, 0, 0, "没有记录");
        }
    }
    else if(1 == log_type)
    {
        jsParent = GetCfgObj(pathOrder, &errcode);
        if (jsParent == NULL)
        {
            cJSON_Delete(jsParent);
            return errcode;
        }
        ulMaxItem  = cJSON_GetArraySize(jsParent);
        if (ulMaxItem == 0)
        {
            cJSON_Delete(jsParent);
            LISTVIEW_AddRow(hItem, NULL);
            LISTVIEW_SetItemText(hItem, 0, 0, "没有记录");
            return 0;
        }
        ts_start.tm_year = (sel_start_date.year[0] - 48) * 1000 +
            (sel_start_date.year[1] - 48) * 100 +
            (sel_start_date.year[2] - 48) * 10 +
            (sel_start_date.year[3] - 48);
        ts_start.tm_mon = (sel_start_date.month[0] - 48) * 10 +
            (sel_start_date.month[1] - 48);
        ts_start.tm_mday = (sel_start_date.day[0] - 48) * 10 +
            (sel_start_date.day[1] - 48);

        ts_end.tm_year = (sel_end_date.year[0] - 48) * 1000 +
            (sel_end_date.year[1] - 48) * 100 +
            (sel_end_date.year[2] - 48) * 10 +
            (sel_end_date.year[3] - 48);
        ts_end.tm_mon = (sel_end_date.month[0] - 48) * 10 +
            (sel_end_date.month[1] - 48);
        ts_end.tm_mday = (sel_end_date.day[0] - 48) * 10 +
            (sel_end_date.day[1] - 48);

        for (j = 0; j < ulMaxItem; j++)
        {

            jsChild = cJSON_GetArrayItem(jsParent, ulMaxItem-j-1);

            jsItem = cJSON_GetObjectItem(jsChild, jnOrderStartTime);
	        ts = localtime((time_t*)&(jsItem->valueint));

            //判断起始年
            if((ts->tm_year+1900) < ts_start.tm_year)
            {
                continue;
            }
            else if((ts->tm_year+1900) == ts_start.tm_year)
            {
                if((ts->tm_mon+1) < ts_start.tm_mon)
                {
                    continue;
                }
                else if((ts->tm_mon+1) == ts_start.tm_mon)
                {
                    if(ts->tm_mday < ts_start.tm_mday)
                    {
                        continue;
                    }
                }
            }
            //判断截止年
            if((ts->tm_year+1900) > ts_end.tm_year)
            {
                continue;
            }
            else if((ts->tm_year+1900) == ts_end.tm_year)
            {
                if((ts->tm_mon+1) > ts_end.tm_mon)
                {
                    continue;
                }
                else if((ts->tm_mon+1) == ts_end.tm_mon)
                {
                    if(ts->tm_mday > ts_end.tm_mday)
                    {
                        continue;
                    }
                }
            }

            /*序号    启动方式    卡号  订单流水号   起始时间    结束时间   结束类型 总电量 总电费 总服务费 总费用 支付方式*/
            LISTVIEW_AddRow(hItem, NULL);
            sprintf((char *)buf, "%d", i+1);
            LISTVIEW_SetItemText(hItem, 0, i, buf);

            jsChild = cJSON_GetArrayItem(jsParent, ulMaxItem-i-1);
            jsItem = cJSON_GetObjectItem(jsChild, jnOrderStartType);
            if(jsItem->valueint == 4)
            {
                LISTVIEW_SetItemText(hItem, 1, i, "刷卡");
            }
            else
            {
                LISTVIEW_SetItemText(hItem, 1, i, "扫码");
            }

	        jsItem = cJSON_GetObjectItem(jsChild, jnCardID);
            LISTVIEW_SetItemText(hItem, 2, i, jsItem->valuestring);


            jsItem = cJSON_GetObjectItem(jsChild, jnOrderOrderSN);
            LISTVIEW_SetItemText(hItem, 3, i, jsItem->valuestring);


            jsItem = cJSON_GetObjectItem(jsChild, jnOrderStartTime);
	        ts = localtime((time_t*)&(jsItem->valueint));
	        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
            LISTVIEW_SetItemText(hItem, 4, i, buf);

	        jsItem = cJSON_GetObjectItem(jsChild, jnOrderStopTime);
	        ts = localtime((time_t*)&(jsItem->valueint));
	        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
            LISTVIEW_SetItemText(hItem, 5, i, buf);

            jsItem = cJSON_GetObjectItem(jsChild, jnOrderStopType);
//            printf_safe("StopType\t%d\n", jsItem->valueint);
            switch(jsItem->valueint)
            {
                case defOrderStopType_RFID:
                    LISTVIEW_SetItemText(hItem, 6, i, "刷卡结束");
                    break;
                case defOrderStopType_Remote:
                    LISTVIEW_SetItemText(hItem, 6, i, "远程结束");
                    break;
                case defOrderStopType_Full:
                    LISTVIEW_SetItemText(hItem, 6, i, "充满结束");
                    break;
                case defOrderStopType_Fee:
                    LISTVIEW_SetItemText(hItem, 6, i, "达到充电金额");
                    break;
                case defOrderStopType_Time:
                    LISTVIEW_SetItemText(hItem, 6, i, "达到充电时间");
                    break;
                case defOrderStopType_Scram:
                    LISTVIEW_SetItemText(hItem, 6, i, "急停结束");
                    break;
                case defOrderStopType_OverCurr:
                    LISTVIEW_SetItemText(hItem, 6, i, "过流结束");
                    break;
                case defOrderStopType_UnPlug:
                    LISTVIEW_SetItemText(hItem, 6, i, "强制拔枪");
                    break;
                case defOrderStopType_Offline:
                case defOrderStopType_Poweroff:
                case defOrderStopType_Knock:
                    LISTVIEW_SetItemText(hItem, 6, i, "异常结束");
                    break;
                default:
                    LISTVIEW_SetItemText(hItem, 6, i, "未知原因结束");
                    break;
            }

            jsItem = cJSON_GetObjectItem(jsChild,jnOrderStopEnergy);
            jsItemTmp = cJSON_GetObjectItem(jsChild,jnOrderStartEnergy);
            sprintf((char *)buf,"%.2lf",(jsItem->valuedouble - jsItemTmp->valuedouble)*100 / 100.0);
            LISTVIEW_SetItemText(hItem, 7, i, buf);

            jsItem = cJSON_GetObjectItem(jsChild, jnOrderTotalEnergyFee);
            sprintf((char *)buf,"%.2lf",jsItem->valuedouble * 100 / 100.0);
            LISTVIEW_SetItemText(hItem, 8, i, buf);

            jsItemTmp = cJSON_GetObjectItem(jsChild, jnOrderTotalServFee);
            sprintf((char *)buf,"%.2lf",jsItemTmp->valuedouble * 100 / 100.0);
            LISTVIEW_SetItemText(hItem, 9, i, buf);

            sprintf((char *)buf,"%.2lf",(jsItem->valuedouble + jsItemTmp->valuedouble) * 100 / 100.0);
            LISTVIEW_SetItemText(hItem, 10, i, buf);

            jsItem = cJSON_GetObjectItem(jsChild, jnOrderPayStatus);
            if(jsItem->valueint == 0)
            {
                LISTVIEW_SetItemText(hItem, 11, i, "未支付");
            }
            else
            {
                LISTVIEW_SetItemText(hItem, 11, i, "已支付");
            }
            i++;
        }
        if (i == 0)
        {
            LISTVIEW_AddRow(hItem, NULL);
            LISTVIEW_SetItemText(hItem, 0, 0, "没有记录");
        }
    }

    cJSON_Delete(jsParent);

    return errcode;
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
//    static int _x,_y;
//    WM_SCROLL_STATE ScrollState;

    switch (pMsg->MsgId)
    {
    case WM_PAINT:

        break;
    case WM_INIT_DIALOG:
        //
        // 初始化window 选择查询时间段日期的window
        //
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5), &SIF36_Font, GUI_BLACK, "起始");
        Text_Show(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6), &SIF36_Font, GUI_BLACK, "终止");
        //
        // Initialization of 'Button'
        //
        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_BLACK,BUTTON_CI_UNPRESSED,GUI_BLACK,"查询告警");

        Button_Show(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5),GUI_TA_HCENTER|GUI_TA_VCENTER,
                    &SIF36_Font,BUTTON_CI_UNPRESSED,GUI_BLACK,BUTTON_CI_UNPRESSED,GUI_BLACK,"查询充电");
        //
        // Initialization of 'Listwheel'
        //
        //初始化listwheel
        for(i = 0;i < 6;i++)
        {
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_0+i);
            //设置字体
            LISTWHEEL_SetFont(hItem, &SIF36_Font);//GUI_FontComic36B_ASCII);
            LISTWHEEL_SetTextAlign(hItem,GUI_TA_VCENTER | GUI_TA_VERTICAL);
            //设置吸附位置
            LISTWHEEL_SetSnapPosition(hItem,80);
            //设置绘制数据项所使用的行高
        //        LISTWHEEL_SetLineHeight(hItem,30);
            //设置选中的条目的文本颜色
            LISTWHEEL_SetTextColor(hItem,LISTWHEEL_CI_SEL,GUI_RED);
        }
        //设置起始年listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_0);
        //绘制指示行
        //    LISTWHEEL_SetOwnerDraw(hItem,ListWheel0_OwnerDraw);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apYear);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apYear+i));
        }
        //LISTWHEEL_SetSel(hItem,0);
        LISTWHEEL_SetPos(hItem,0);

        //设置起始月listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_1);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apMonth);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apMonth+i));
        }
        LISTWHEEL_SetSel(hItem,0);
        LISTWHEEL_SetPos(hItem,0);

        //设置起始日listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_2);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apDay);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apDay+i));
        }
        LISTWHEEL_SetSel(hItem,0);
        LISTWHEEL_SetPos(hItem,0);

        //设置终止年listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_3);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apYear);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apYear+i));
        }
        LISTWHEEL_SetSel(hItem,3);
        LISTWHEEL_SetPos(hItem,3);

        //设置终止月listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_4);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apMonth);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apMonth+i));
        }
        LISTWHEEL_SetSel(hItem,0);
        LISTWHEEL_SetPos(hItem,0);

        //设置终止日listwheel
        hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_5);
        //添加数据
        for(i = 0;i < GUI_COUNTOF(_apDay);i++)
        {
            LISTWHEEL_AddString(hItem,*(_apDay+i));
        }
        LISTWHEEL_SetSel(hItem,0);
        LISTWHEEL_SetPos(hItem,0);

        //
        // 初始列表控件
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        /* 设置列表控件中header控件的所显示文本的字体 */
        hHeader = LISTVIEW_GetHeader(hItem);
        HEADER_SetFont(hHeader, &SIF16_Font);
//            LISTVIEW_SetAutoScrollH(hItem,1);
//            LISTVIEW_SetAutoScrollV(hItem,1);

        /*srollbar*/
        hScroll = SCROLLBAR_CreateAttached(hItem, 0);//水平滑轮
        SCROLLBAR_SetNumItems(hScroll, 30 * 4);
        SCROLLBAR_SetWidth(hScroll,20);
        wScroll = SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);//垂直滑轮
        SCROLLBAR_SetNumItems(wScroll, 30 * 20);
        SCROLLBAR_SetWidth(wScroll,20);
        /*end*/

        //SCROLLBAR_SetDefaultWidth(15);
        /* 设置列表控件选项中所显示文本的字体 */
        LISTVIEW_SetFont(hItem, &SIF16_Font);
        /* 设置列表控件表格可见 */
        LISTVIEW_SetGridVis(hItem, 1);
        WM_HideWindow(hItem);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        BUTTON_SetFont(hItem, &SIF16_Font);
        WM_HideWindow(hItem);
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
            /*scrollbar*/
//        case GUI_ID_HSCROLL:
//            if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
//            {
//                   /* 得到滚动条的状态，得到的数值好像是负值 才能使得 _x - ScrollState.v是正值 */
//                    WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
//                    if (_x != ScrollState.v)
//                    {
//                        WM_MoveWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), _x - ScrollState.v, 0);
//                        _x = ScrollState.v;
//                    }
//            }
//            else if(NCode == GUI_ID_VSCROLL)
//            {
//                    WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
//                    if (_y != ScrollState.v)
//                    {
//                        WM_MoveWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),0, _y - ScrollState.v);
//                        _y = ScrollState.v;
//                    }
//            }
//            break;
            /*end*/
        case ID_BUTTON_0: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            /**< 跳转到模拟量信息查询 */
//            _deleteWin(_hWinManagerAlarmLog);
//            CreateManagerInfoAnalog();
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_BUTTON_1: // Notifications sent by 'Button'
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
        case ID_BUTTON_2: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            /**< 跳转到设置参数信息查询 */
//            _deleteWin(_hWinManagerAlarmLog);
            //CreateManagerSysSet();
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
            //WM_DeleteWindow(pMsg->hWin);
            //PutOut_SelAOrB();
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
              //隐藏查询时间button，listview，展示时间滚轮
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_3));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_4));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_5));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4));
              WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5));

              WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3));
              WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
              // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_BUTTON_4: //查询故障 Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            //删除window 创建listview的window，并且初始化listview和填充值
//            GUI_EndDialog(_hWinManagerLogDate,0);
//            CreateManagerLog(0);
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_3));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_4));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_5));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5));

            WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3));
            WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));

            /**< get到现有表格的行列数 */
            column_num = LISTVIEW_GetNumColumns(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
            for(i = 0;i < column_num;i++)
            {
                LISTVIEW_DeleteColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),0);
            }
            row_num = LISTVIEW_GetNumRows(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
            for (i = 0; i < row_num; i++)
            {
                LISTVIEW_DeleteRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0);
            }
            /*序号 记录时间  枪号  故障等级  故障状态  故障信息*/
			/* 添加四列表，调用一次函数LISTVIEW_AddColumn添加一列 */
            LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "序号", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "记录时间", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 60, "设备", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "故障等级", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "故障状态", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "故障信息", GUI_TA_HCENTER | GUI_TA_VCENTER);

			/* 添加三行，调用一次函数LISTVIEW_AddRow添加一行 */
//			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), NULL);
//			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), NULL);
//			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), NULL);


            Data_Flush(0,WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
//            for(i = 0; i< 500;i++)
//            {
//                LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), NULL);
//                sprintf(buf, "%03d", i+1);
//                //序号
//                LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0, i, buf);
////                memset(buf,'\0',strlen(buf));
//                strcpy(buf, "2016-07-07 12:13:14");
//                LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 1, i, buf);
////                memset(buf,'\0',strlen(buf));
//                strcpy(buf, "2016-07-07 12:13:14");
//                LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 2, i, buf);
////                memset(buf,'\0',strlen(buf));
//                strcpy(buf, "i");
//                LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 3, i, buf);
//            }

			/* 为列表控件三行四列共12个选项全部添加文本 */
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0, 0, "00");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 1, 0, "2016-07-07 12:13:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 2, 0, "2016-07-07 12:14:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 3, 0, "急停故障");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0, 1, "01");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 1, 1, "2016-07-07 12:13:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 2, 1, "2016-07-07 12:13:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 3, 1, "交流过压");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0, 2, "02");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 1, 2, "2016-07-07 12:13:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 2, 2, "2016-07-07 12:13:14");
//			LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 3, 2, "继电器故障");
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_BUTTON_5: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_3));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_4));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_5));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_5));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_TEXT_6));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4));
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5));

            WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3));
            WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));

            column_num = LISTVIEW_GetNumColumns(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
            for(i = 0;i < column_num;i++)
            {
                LISTVIEW_DeleteColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0),0);
            }

            row_num = LISTVIEW_GetNumRows(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));
            for (i = 0; i < row_num; i++)
            {
                LISTVIEW_DeleteRow(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 0);
            }

            /*序号    启动方式    卡号  订单流水号   起始时间    结束时间   结束类型 总电量 总电费 总服务费 总费用 支付方式*/
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "序号", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 60, "启动方式", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "卡号", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 240, "订单流水号", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "起始时间", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "结束时间", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 160, "结束类型", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "总电量", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "总电费", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "总服务费", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "总费用", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0), 80, "支付状态", GUI_TA_HCENTER | GUI_TA_VCENTER);
            Data_Flush(1,WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0));

            break;
        /// TODO (zshare#1#): ///因为鼠标重置的问题, \
        所以需要用到WM_NOTIFICATION_MOVED_OUT事件,不触发WM_NOTIFICATION_RELEASED
          case WM_NOTIFICATION_RELEASED://WM_NOTIFICATION_MOVED_OUT:
            /**< 跳转到状态信息查询 */
//            _deleteWin(_hWinManagerInfoAnalog);
//            CreateManagerInfoStatus();
            break;
          }
            break;
        case ID_LISTWHEEL_0: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
//              LISTWHEEL_SetVelocity(hItem, 1000);
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            //获取句柄
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_0);
            //获取当前句柄索引
            list_start_index[0] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            //LISTWHEEL_SetSel(hItem,list_start_index[0]);
            LISTWHEEL_GetItemText(hItem,list_start_index[0],sel_start_date.year,5);
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_LISTWHEEL_1: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            //获取句柄
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_1);
            //获取当前句柄索引
            list_start_index[1] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            LISTWHEEL_SetSel(hItem,list_start_index[1]);
            LISTWHEEL_GetItemText(hItem,list_start_index[1],sel_start_date.month,5);
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_LISTWHEEL_2: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            //获取句柄
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_2);
            //获取当前句柄索引
            list_start_index[2] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            LISTWHEEL_SetSel(hItem,list_start_index[2]);
            LISTWHEEL_GetItemText(hItem,list_start_index[2],sel_start_date.day,5);
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_LISTWHEEL_3: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_3);
            //获取当前句柄索引
            list_end_index[0] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            LISTWHEEL_SetSel(hItem,list_end_index[0]);
            LISTWHEEL_GetItemText(hItem,list_end_index[0],sel_end_date.year,5);
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_LISTWHEEL_4: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_4);
            //获取当前句柄索引
            list_end_index[1] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            LISTWHEEL_SetSel(hItem,list_end_index[1]);
            LISTWHEEL_GetItemText(hItem,list_end_index[1],sel_end_date.month,5);
            // USER END
            break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
          }
          break;
        case ID_LISTWHEEL_5: // Notifications sent by 'Listwheel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
          case WM_NOTIFICATION_SEL_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_5);
            //获取当前句柄索引
            list_end_index[2] = LISTWHEEL_GetPos(hItem);
            //设置当前句柄数据项
            LISTWHEEL_SetSel(hItem,list_end_index[2]);
            LISTWHEEL_GetItemText(hItem,list_end_index[2],sel_end_date.day,5);
            // USER END
            break;
        }
        break;
    }
    case MSG_DELETEMANAGERWIN:
        GUI_EndDialog(_hWinManagerLog,0);
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
 *  输出管理员界面里的信息查询记录选择日期
 * @param
 * @param
 * @return
 *       CreateManagerLogDate
*/
WM_HWIN CreateManagerLogDate(WM_HWIN srcHwin)
{
    _hWinManagerLogDate = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_GetClientWindow(srcHwin), 0, 0);
    return _hWinManagerLogDate;
}
/*************************** End of file ****************************/


