﻿/**
* @file Touch_Disp_Module.c
* @brief 封装控件的显示函数
* @author share
* @version v1.0
* @date 2017-03-02
*/
#include "xbffontcreate.h"
#include "touchtimer.h"
#include "DIALOG.h"

uint8_t *Scram_err = "急停异常\n";
uint8_t *Volt_err = "充电电压异常\n";
uint8_t *ACTemp_err = "温度异常\n";
uint8_t *PE_err = "PE异常\n";
uint8_t *Knock_err = "撞击异常\n";
uint8_t *Arrester_err = "防雷异常\n";
uint8_t *PowerOff_err = "停电异常\n";
uint8_t *Curr_err = "电流异常\n";
uint8_t *Freq_err = "频率异常\n";

WM_HWIN err_hItem;

//uint8_t bitset(uint32_t var,uint8_t bitno)            //置位
//{
//    return ((var) |= (1<<(bitno)));
//}
//
//uint8_t bitclr(uint32_t var,uint8_t bitno)
//{
//    return ((var) &= ~(1<<(bitno)));         //清0
//}
//
//uint8_t bittest(uint32_t var,uint8_t bitno)
//{
//    return ((var >> bitno)& 0x01);           //位状态检测
//}


/** framewin初始化
 *
 * pMsg:消息体 ,textid0:文本0 ,textid1:文本1, textid2：文本2，textid3:文本3
 * @param
 * @return
 *
 */
void FrameWin_Init(WM_MESSAGE *pMsg,uint16_t textid0,uint16_t textid1,uint16_t textid2,uint16_t textid3)
{
    FrameWin_Show(pMsg->hWin,GUI_TA_HCENTER | GUI_TA_VCENTER,40,&XBF24_Font,GUI_RED,"欢迎使用北京动力源交流充电桩");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid2),&XBF16_Font,GUI_RED,"信号：强");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid3),&XBF16_Font,GUI_BLACK,"感谢您为空气的清新奉献一份力量");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid0),&XBF16_Font,GUI_RED,"2017-02-28");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid1),&XBF16_Font,GUI_RED,"14:00:00");
    WM_SetFocus(pMsg->hWin);
}
/** window初始化
 *
 * pMsg:消息体 ,textid0:文本0 ,textid1:文本1, textid2：文本2，textid3:文本3
 * @param
 * @return
 *
 */
void Window_Init(WM_MESSAGE *pMsg,uint16_t textid0,uint16_t textid1,uint16_t textid2,uint16_t textid3)
{
    //FrameWin_Show(pMsg->hWin,GUI_TA_HCENTER | GUI_TA_VCENTER,40,&XBF24_Font,GUI_RED,"欢迎使用北京动力源交流充电桩");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid2),&XBF16_Font,GUI_RED,"信号：强");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid3),&XBF16_Font,GUI_BLACK,"感谢您为空气的清新奉献一份力量");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid0),&XBF16_Font,GUI_RED,"2017-02-28");
    Text_Show(WM_GetDialogItem(pMsg->hWin, textid1),&XBF16_Font,GUI_RED,"14:00:00");
    WM_SetFocus(pMsg->hWin);
}
/** framewin初始化
 *
 * pMsg:消息体 ,textid0:文本0 ,textid1:文本1, textid2：文本2，textid3:文本3
 * @param
 * @return
 *
 */
void Caculate_RTC_Show(WM_MESSAGE *pMsg,uint16_t textid0,uint16_t textid1)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    WM_HWIN hWin = pMsg->hWin;

    HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
    xsprintf((char *)Date_buf, "20%02d-%02d-%02d", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
    TEXT_SetText(WM_GetDialogItem(hWin, textid0), Date_buf);

    HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
    xsprintf((char *)Time_buf, "%02d:%02d:%02d", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
    TEXT_SetText(WM_GetDialogItem(hWin, textid1), Time_buf);
}
/** framewin初始化
 *
 * WM_WIN:源窗口 ,align:对齐方式,heigh:窗口高度,font:字体类型,color:字体颜色,buf:显示的文字内容
 * @param
 * @return
 *
 */
void FrameWin_Show(WM_HWIN hItem,uint8_t aglin,uint8_t heigh,GUI_FONT *font,uint32_t color,uint8_t *buf)
{
    FRAMEWIN_SetTextAlign(hItem, aglin);
    FRAMEWIN_SetTitleHeight(hItem, heigh);
    FRAMEWIN_SetFont(hItem, font);
    FRAMEWIN_SetTextColor(hItem, color);
    FRAMEWIN_SetText(hItem, buf);
//    FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
}
/** Text显示
 *
 * WM_WIN:源窗口,font:字体类型,color:字体颜色,buf:显示的文字内容
 * @param
 * @return
 *
 */
void Text_Show(WM_HWIN hItem,GUI_FONT *font,uint32_t color,uint8_t *buf)
{
    TEXT_SetFont(hItem, font);
    TEXT_SetTextColor(hItem, color);
    TEXT_SetText(hItem, buf);
}
/** Edit用作带外方框的显示
 *
 * WM_WIN:源窗口,font:字体类型,color:字体颜色,buf:显示的文字内容
 * @param
 * @return
 * note；此处并不做编辑处理,没有做颜色处理
 */
void Edit_Show(WM_HWIN hItem,GUI_FONT *font,uint8_t *buf)
{
    EDIT_SetFont(hItem, font);
    EDIT_SetText(hItem, buf);
}
/** Button显示
 *
 * WM_WIN:源窗口,aglin:文字对齐方式,font:字体类型,bkcolor:按钮背景颜色 ,color:字体颜色,buf:显示的文字内容
 * @param
 * @return
 *
 */
void Button_Show(WM_HWIN hItem,uint8_t aglin,GUI_FONT *font,uint8_t bk_style,uint32_t bkcolor,uint8_t text_style,uint32_t color,uint8_t *buf)
{
    BUTTON_SetFont(hItem, font);
    BUTTON_SetTextAlign(hItem,aglin);
    BUTTON_SetBkColor(hItem, bk_style, bkcolor);
    BUTTON_SetTextColor(hItem, text_style, color);
    BUTTON_SetText(hItem, buf);
}
/** 图片显示
 *
 * WM_WIN:源窗口,aglin:文字对齐方式,font:字体类型,bkcolor:按钮背景颜色 ,color:字体颜色,buf:显示的文字内容
 * @param
 * @return
 *Note:主要针对于二维码
 */
void Image_Show(WM_HWIN hItem,uint8_t imageid,U32 filesize)
{
    const void  *pData;

    //pData = _GetImageById(imageid, &filesize);
    IMAGE_SetBMP(hItem, pData, filesize);
}
/** 解析置位数据
 * bit3 : 显示故障界面的控件
 * bit4 : 删除故障界面的控件
 * bit5 : 重新校准
 * bit6 : 跳转首页选枪界面
 * bit7 : 跳转管理员密码界面
 * WM_WIN:源窗口
 * @param
 * @return
 *Note:
 */
void CaliDone_Analy(WM_HWIN hWin)//Jump_IsManager(WM_HWIN hWin)
{
    if(bittest(calebrate_done,4))
    {
        bitclr(calebrate_done,4);
        WM_DeleteWindow(err_hItem);
    }
    if(bittest(calebrate_done,5))
    {
        bitclr(calebrate_done,5);
        WM_DeleteWindow(hWin);
        vTaskDelay(100);
        LCD_Init();
        TP_Init();
        vTaskDelay(100);
        LCD_Clear(WHITE);
        TP_Adjust();
        bitset(calebrate_done,0);
        PutOut_SelAOrB();
    }
    if(bittest(calebrate_done,6))
    {
        bitclr(calebrate_done,6);
        WM_DeleteWindow(hWin);
        PutOut_SelAOrB();
    }
    if(bittest(calebrate_done,7))
    {
        bitclr(calebrate_done,7);
        WM_DeleteWindow(hWin);
        Keypad_GetValue(LOGIN_PASSWD);
    }
}
/** 生成二维码数据
 *qrcode_data:需要生成二维码的字符串，p计算画笔的大小，x计算x坐标，y计算y坐标，
 *
 * @param
 * @return
 *Note:
 */
void qrencode(uint8_t *qrcode_data,uint16_t *p,uint16_t *x,uint16_t *y)
{
	uint8_t i;

	EncodeData((char *)qrcode_data);

    LCD_Fill(130,170,130+m_nSymbleSize,170+m_nSymbleSize,WHITE);

	for(i=0;i<10;i++)
	{
		if((m_nSymbleSize*i*2)>(130+m_nSymbleSize))	break;
	}

	*p=(i-1)*2;//点大小
	if(m_nSymbleSize <= 250)
    {
        *x = 67 + ((250 - m_nSymbleSize*(*p)) / 2);
    }
    else
    {
        *x = 67 - ((m_nSymbleSize*(*p) - 250) / 2);
    }

	*y = 170;
}
/** 画二维码
 *x:显示二维码的左上角x 坐标，y:显示二维码的左上角y坐标，p：画笔的大小
 *
 * @param
 * @return
 *Note:
 */
void display_encode(uint16_t *x,uint16_t *y,uint16_t *p)
{
 	uint8_t i,j;

	for(i=0;i<m_nSymbleSize;i++)
	{
		for(j=0;j<m_nSymbleSize;j++)
		{
			if(m_byModuleData[i][j]==1)
            {
                LCD_Fill(*x+(*p)*i,*y+(*p)*j,*x+(*p)*(i+1)-1,*y+(*p)*(j+1)-1,BLACK);
            }
		}
	}
}
/** @brief 故障弹窗弹出展示
 *
 * @param
 * @param
 * @return
 *
 */
void ErrWindow_Show(WM_HWIN hWin)
{
    CON_t *pCON;
    static EventBits_t uxBitsErr;
    static EventBits_t uxBitsErrTmp;

    pCON = CONGetHandle(0);
    uxBitsErr = xEventGroupGetBits(pCON->status.xHandleEventCharge);

    if((uxBitsErr & 0x0003e21c) != 0)//0x3C21C)
    {
        if(uxBitsErrTmp != (uxBitsErr & 0x0003e21c))
        {
            uxBitsErrTmp = (uxBitsErr & 0x0003e21c);
            //WM_SendMessageNoPara(hWin, WM_NOTIFY_PARENT);
            err_window(hWin,uxBitsErr);
        }
    }
    if(bittest(calebrate_done,3) == 1)
    {
        bitclr(calebrate_done,3);
        err_window(hWin,uxBitsErr);
    }
}

/** @brief 故障弹窗内容组装
 *
 * @param
 * @param
 * @return
 *
 */
void err_window(WM_HWIN hWin,EventBits_t uxBitsErr)
{

    uint8_t msg_err[150] = "\0";

    WM_DeleteWindow(err_hItem);
    err_hItem = MULTIEDIT_CreateEx(460, 60, 300, 300, WM_GetClientWindow(hWin), WM_CF_SHOW, 0, GUI_ID_MULTIEDIT0, 100, NULL);
    MULTIEDIT_SetInsertMode(err_hItem,1);  //开启插入模式
    MULTIEDIT_SetFont(err_hItem, &XBF24_Font);
    WM_SetFocus(err_hItem);
    MULTIEDIT_SetInsertMode(err_hItem, 1);
    MULTIEDIT_SetCursorOffset(err_hItem,0);
    MULTIEDIT_EnableBlink(err_hItem,0,0);

    if((uxBitsErr >> 13) & 0x01)//(bittest(err_sysbol,defEventBitEVSEScramOK))
    {
        strncat(msg_err,Scram_err,strlen(Scram_err));
        //MULTIEDIT_SetText(hItem, "急停异常\n");
    }
    if((uxBitsErr >> 2) & 0x01)
    //if(bittest(err_sysbol,defEventBitCONVoltOK))
    {
        strncat(msg_err,Volt_err,strlen(Volt_err));
        //MULTIEDIT_SetText(hItem, "充电电压异常\n");
    }
    if((uxBitsErr >> 9) & 0x01)
    //if(bittest(err_sysbol,defEventBitCONACTempOK))
    {
        strncat(msg_err,ACTemp_err,strlen(ACTemp_err));
        //MULTIEDIT_SetText(hItem, "温度异常\n");
    }
    if((uxBitsErr >> 14) & 0x01)
    //if(bittest(err_sysbol,defEventBitEVSEPEOK))
    {
        strncat(msg_err,PE_err,strlen(PE_err));
        //MULTIEDIT_SetText(hItem, "PE异常\n");
    }
    if((uxBitsErr >> 15) & 0x01)
    //if(bittest(err_sysbol,defEventBitEVSEKnockOK))
    {
        strncat(msg_err,Knock_err,strlen(Knock_err));
        //MULTIEDIT_SetText(hItem, "撞击异常\n");
    }
    if((uxBitsErr >> 16) & 0x01)
    //if(bittest(err_sysbol,defEventBitEVSEArresterOK))
    {
        strncat(msg_err,Arrester_err,strlen(Arrester_err));
        //MULTIEDIT_SetText(hItem, "防雷异常\n");
    }
    if((uxBitsErr >> 17) & 0x01)
    //if(bittest(err_sysbol,defEventBitEVSEPowerOffOK))
    {
        strncat(msg_err,PowerOff_err,strlen(PowerOff_err));
       // MULTIEDIT_SetText(hItem, "停电异常\n");
    }
    if((uxBitsErr >> 3) & 0x01)
    //if(bittest(err_sysbol,defEventBitCONCurrOK))
    {
        strncat(msg_err,Curr_err,strlen(Curr_err));
        //MULTIEDIT_SetText(hItem, "电流异常\n");
    }
    if((uxBitsErr >> 4) & 0x01)
    //if(bittest(err_sysbol,defEventBitCONFreqOK))
    {
        strncat(msg_err,Freq_err,strlen(Freq_err));
       // MULTIEDIT_SetText(hItem, "频率异常\n");
    }
    MULTIEDIT_SetText(err_hItem, msg_err);
//    SCROLLBAR_CreateAttached(err_hItem, SCROLLBAR_CF_VERTICAL);//创建垂直滑轮
//    SCROLLBAR_CreateAttached(err_hItem, 0);//创建水平滑轮

//    GUI_CURSOR_SetPosition(460,160);
    WM_SetFocus(err_hItem);
    bitclr(calebrate_done,4);
}





