#include "bsp_define.h"
#include "touch.h"
#include "lcddrv.h"
#include "stdlib.h"
#include "math.h"
#include "GUI.h"
#include "cJSON.h"
#include "cfg_parse.h"
#include "ff.h"
#include "errorcode.h"
#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utils.h"
#include <string.h>

static float sqrt1(float x)
{
    float xhalf = 0.5 * x;
    int i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x*(1.5-xhalf*x*x);
    x = x*(1.5-xhalf*x*x);
    x = x*(1.5-xhalf*x*x);

    return 1/x;
}
_m_tp_dev tp_dev =
{
    TP_Init,
    TP_Scan,
    TP_Adjust,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
//默认为touchtype=0的数据.
uint8_t CMD_RDX = 0XD0;
uint8_t CMD_RDY = 0X90;

//SPI写数据
//向触摸屏IC写入1byte数据
//num:要写入的数据
void TP_Write_Byte(uint8_t num)
{
    uint8_t count = 0;
    for(count = 0; count < 8; count++)
    {
        if(num & 0x80)
        {
            TDIN = 1;
        }
        else
        {
            TDIN = 0;
        }
        num <<= 1;
        TCLK = 0;
        bsp_DelayUS(1);
        TCLK = 1;   //上升沿有效
    }
}
//SPI读数据
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据
uint16_t TP_Read_AD(uint8_t CMD)
{
    uint8_t count = 0;
    uint16_t Num = 0;
    TCLK = 0;   //先拉低时钟
    TDIN = 0;   //拉低数据线
    TCS = 0;    //选中触摸屏IC
    TP_Write_Byte(CMD);//发送命令字
    bsp_DelayUS(3);//ADS7846的转换时间最长为6us
    TCLK = 0;
    bsp_DelayUS(1);
    TCLK = 1;   //给1个时钟，清除BUSY
    bsp_DelayUS(1);
    TCLK = 0;
    for(count = 0; count < 16; count++) //读出16位数据,只有高12位有效
    {
        Num <<= 1;
        TCLK = 0; //下降沿有效
        bsp_DelayUS(1);
        TCLK = 1;
        if(DOUT)
        {
            Num++;
        }
    }
    Num >>= 4;  //只有高12位有效.
    TCS = 1;    //释放片选
    return(Num);
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 15    //读取次数
#define LOST_VAL 1      //丢弃值
uint16_t TP_Read_XOY(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum = 0;
    uint16_t temp;
    for(i = 0; i < READ_TIMES; i++)
    {
        buf[i] = TP_Read_AD(xy);
    }
    for(i = 0; i < READ_TIMES - 1; i++) //排序
    {
        for(j = i + 1; j < READ_TIMES; j++)
        {
            if(buf[i] > buf[j]) //升序排列
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for(i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)
    {
        sum += buf[i];
    }
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y)
{
    uint16_t xtemp, ytemp;
    xtemp = TP_Read_XOY(CMD_RDX);
    ytemp = TP_Read_XOY(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;//读数失败
    *x = xtemp;
    *y = ytemp;
    return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y)
{
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t flag;
    flag = TP_Read_XY(&x1, &y1);
    if(flag == 0)
    {
        return(0);
    }

    flag = TP_Read_XY(&x2, &y2);
    if(flag == 0)
    {
        return(0);
    }
    if(((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) //前后两次采样在+-50内
            && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    }
    else
    {
        return 0;
    }
}
//////////////////////////////////////////////////////////////////////////////////
//与LCD部分有关的函数
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color)
{
    POINT_COLOR = color;
    LCD_DrawLine(x - 12, y, x + 13, y); //横线
    LCD_DrawLine(x, y - 12, x, y + 13); //竖线
    LCD_DrawPoint(x + 1, y + 1);
    LCD_DrawPoint(x - 1, y + 1);
    LCD_DrawPoint(x + 1, y - 1);
    LCD_DrawPoint(x - 1, y - 1);
    LCD_Draw_Circle(x, y, 6); //画中心圈
}
//画一个大点(2*2的点)
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
    POINT_COLOR = color;
    LCD_DrawPoint(x, y); //中心点
    LCD_DrawPoint(x + 1, y);
    LCD_DrawPoint(x, y + 1);
    LCD_DrawPoint(x + 1, y + 1);
}
void TP_Scan_Get(u16 *adc_x, u16 *adc_y)
{
    u8 tp_pressed;
    u8 invalid_count = 0;
    tp_pressed = PEN ? 0 : 1;
    //      1, 触摸屏按下
    //      0, 触摸屏释放
    if(tp_pressed == 1)
    {
        while(TP_Read_XY2(adc_x, adc_y) != 1 && invalid_count < 20)
        {
            invalid_count++;
        }
        if(invalid_count >= 20)
        {
            *adc_x = 0xffff;
            *adc_y = 0xffff;
        }

    }
    else
    {
        *adc_x = 0xffff;
        *adc_y = 0xffff;
    }
}
uint8_t TP_Read_Pressure(float *pressure)
{
    uint16_t x, y, z1;
    float xTPr = 738, yTPr = 222;
    z1 = TP_Read_XOY(0xB4);
    TP_Read_XY2(&x, &y);
    *pressure = (xTPr * (float)x) / 4096.0f * (4096.0f / (float)z1 - 1) - yTPr * (1 - (float)y / 4096.0f);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t TP_Scan(uint8_t tp)
{
    if(PEN == 0) //有按键按下
    {
        if(tp)
        {
            TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);    //读取物理坐标
        }
        else if(TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0])) //读取屏幕坐标
        {
            tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] + tp_dev.xoff; //将结果转换为屏幕坐标
            tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
        }
        if((tp_dev.sta & TP_PRES_DOWN) == 0) //之前没有被按下
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES; //按键按下
            tp_dev.x[4] = tp_dev.x[0]; //记录第一次按下时的坐标
            tp_dev.y[4] = tp_dev.y[0];
        }
    }
    else
    {
        if(tp_dev.sta & TP_PRES_DOWN) //之前是被按下的
        {
            tp_dev.sta &= ~(1 << 7); //标记按键松开
        }
        else //之前就没有被按下
        {
            tp_dev.x[4] = 0;
            tp_dev.y[4] = 0;
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
        }
    }
    return tp_dev.sta & TP_PRES_DOWN; //返回当前的触屏状态
}
//提示字符串
uint8_t *const TP_REMIND_MSG_TBL = "Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.If you can't click the red cross within 30 seconds,the program of calibration will quit!";

//提示校准结果(各个参数)
void TP_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac)
{
    POINT_COLOR = RED;
    LCD_ShowString(500, 160, lcddev.width, lcddev.height, 16, "x1:");
    LCD_ShowString(500 + 80, 160, lcddev.width, lcddev.height, 16, "y1:");
    LCD_ShowString(500, 180, lcddev.width, lcddev.height, 16, "x2:");
    LCD_ShowString(500 + 80, 180, lcddev.width, lcddev.height, 16, "y2:");
    LCD_ShowString(500, 200, lcddev.width, lcddev.height, 16, "x3:");
    LCD_ShowString(500 + 80, 200, lcddev.width, lcddev.height, 16, "y3:");
    LCD_ShowString(500, 220, lcddev.width, lcddev.height, 16, "x4:");
    LCD_ShowString(500 + 80, 220, lcddev.width, lcddev.height, 16, "y4:");
    LCD_ShowString(500, 240, lcddev.width, lcddev.height, 16, "fac is:");
    LCD_ShowNum(500 + 24, 160, x0, 4, 16); //显示数值
    LCD_ShowNum(500 + 24 + 80, 160, y0, 4, 16); //显示数值
    LCD_ShowNum(500 + 24, 180, x1, 4, 16); //显示数值
    LCD_ShowNum(500 + 24 + 80, 180, y1, 4, 16); //显示数值
    LCD_ShowNum(500 + 24, 200, x2, 4, 16); //显示数值
    LCD_ShowNum(500 + 24 + 80, 200, y2, 4, 16); //显示数值
    LCD_ShowNum(500 + 24, 220, x3, 4, 16); //显示数值
    LCD_ShowNum(500 + 24 + 80, 220, y3, 4, 16); //显示数值
    LCD_ShowNum(500 + 56, 240, fac, 3, 16); //显示数值,该数值必须在95~105范围之内.

}

//触摸屏校准代码
//得到四个校准参数
void TP_Adjust(void)
{
    uint16_t pos_temp[4][2];//坐标缓存值
    uint8_t  cnt = 0;
    uint16_t d1, d2;
    uint32_t tem1, tem2;
    double fac;
    uint16_t outtime = 0;
    uint8_t tmp[4];
	
	tp_dev.touchtype = 0;
	if (tp_dev.touchtype)//X,Y方向与屏幕相反
	{
		CMD_RDX = 0X90;
		CMD_RDY = 0XD0;
	}
	else                   //X,Y方向与屏幕相同
	{
		CMD_RDX = 0XD0;
		CMD_RDY = 0X90;
	}
	
    cnt = 0;
    POINT_COLOR = BLUE;
    BACK_COLOR = WHITE;
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = RED; //红色
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = RED;
    LCD_ShowString(210, 150, 200, 16, 16, "DPC TOUCH CALIBRATE");
    LCD_ShowString(210, 200, 250, 100, 16, (uint8_t *)TP_REMIND_MSG_TBL); //显示提示信息
    TP_Drow_Touch_Point(20, 20, RED); //画点1
    tp_dev.sta = 0; //消除触发信号
    tp_dev.xfac = 0; //xfac用来标记是否校准过,所以校准之前必须清掉!以免错误
    while(1)//如果连续30秒钟没有按下,则自动退出
    {
        tp_dev.scan(1);//扫描物理坐标
        if((tp_dev.sta & 0xc0) == TP_CATH_PRES) //按键按下了一次(此时按键松开了.)
        {
            outtime = 0;
            tp_dev.sta &= ~(1 << 6); //标记按键已经被处理过了.

            pos_temp[cnt][0] = tp_dev.x[0];
            pos_temp[cnt][1] = tp_dev.y[0];
            cnt++;
            switch(cnt)
            {
                case 1:
                    TP_Drow_Touch_Point(20, 20, WHITE);             //清除点1
                    TP_Drow_Touch_Point(lcddev.width - 20, 20, RED); //画点2
                    break;
                case 2:
                    TP_Drow_Touch_Point(lcddev.width - 20, 20, WHITE); //清除点2
                    TP_Drow_Touch_Point(20, lcddev.height - 20, RED); //画点3
                    break;
                case 3:
                    TP_Drow_Touch_Point(20, lcddev.height - 20, WHITE);     //清除点3
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, RED); //画点4
                    break;
                case 4:  //全部四个点已经得到
                    //对边相等
                    tem1 = utils_abs(pos_temp[0][0] - pos_temp[1][0]); //x1-x2
                    tem2 = utils_abs(pos_temp[0][1] - pos_temp[1][1]); //y1-y2
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d1 = sqrt(tem1 + tem2); //得到1,2的距离

                    tem1 = utils_abs(pos_temp[2][0] - pos_temp[3][0]); //x3-x4
                    tem2 = utils_abs(pos_temp[2][1] - pos_temp[3][1]); //y3-y4
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d2 = sqrt(tem1 + tem2); //得到3,4的距离
                    fac = (float)d1 / d2;
                    if(fac < 0.80 || fac > 1.2 || d1 == 0 || d2 == 0) //不合格
                    {
                        cnt = 0;
                        TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                        TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                        TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                        continue;
                    }
                    tem1 = utils_abs(pos_temp[0][0] - pos_temp[2][0]); //x1-x3
                    tem2 = utils_abs(pos_temp[0][1] - pos_temp[2][1]); //y1-y3
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d1 = sqrt(tem1 + tem2); //得到1,3的距离

                    tem1 = utils_abs(pos_temp[1][0] - pos_temp[3][0]); //x2-x4
                    tem2 = utils_abs(pos_temp[1][1] - pos_temp[3][1]); //y2-y4
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d2 = sqrt(tem1 + tem2); //得到2,4的距离
                    fac = (float)d1 / d2;
                    if(fac < 0.80 || fac > 1.20) //不合格
                    {
                        cnt = 0;
                        TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                        TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                        TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                        continue;
                    }//正确了

                    //对角线相等
                    tem1 = utils_abs(pos_temp[1][0] - pos_temp[2][0]); //x1-x3
                    tem2 = utils_abs(pos_temp[1][1] - pos_temp[2][1]); //y1-y3
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d1 = sqrt(tem1 + tem2); //得到1,4的距离

                    tem1 = utils_abs(pos_temp[0][0] - pos_temp[3][0]); //x2-x4
                    tem2 = utils_abs(pos_temp[0][1] - pos_temp[3][1]); //y2-y4
                    tem1 *= tem1;
                    tem2 *= tem2;
                    d2 = sqrt(tem1 + tem2); //得到2,3的距离
                    fac = (float)d1 / d2;
                    if(fac < 0.80 || fac > 1.2) //不合格
                    {
                        cnt = 0;
                        TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                        TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                        TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                        continue;
                    }//正确了
                    //计算结果
                    tp_dev.xfac = (float)(lcddev.width - 40) / (pos_temp[1][0] - pos_temp[0][0]); //得到xfac
                    tp_dev.xoff = (lcddev.width - tp_dev.xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2; //得到xoff

                    tp_dev.yfac = (float)(lcddev.height - 40) / (pos_temp[2][1] - pos_temp[0][1]); //得到yfac
                    tp_dev.yoff = (lcddev.height - tp_dev.yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2; //得到yoff
                    if(utils_abs(tp_dev.xfac) > 2 || utils_abs(tp_dev.yfac) > 2) //触屏和预设的相反了.
                    {
                        cnt = 0;
                        TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                        TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                        LCD_ShowString(40, 26, lcddev.width, lcddev.height, 16, "TP Need readjust!");
                        tp_dev.touchtype = !tp_dev.touchtype; //修改触屏类型.
                        if(tp_dev.touchtype)//X,Y方向与屏幕相反
                        {
                            CMD_RDX = 0X90;
                            CMD_RDY = 0XD0;
                        }
                        else                   //X,Y方向与屏幕相同
                        {
                            CMD_RDX = 0XD0;
                            CMD_RDY = 0X90;
                        }
                        continue;
                    }
                    POINT_COLOR = BLUE;
                    LCD_Clear(WHITE);//清屏
                    LCD_ShowString(210, 150, lcddev.width, lcddev.height, 16, "Touch Screen Adjust OK!"); //校正完成
                    vTaskDelay(1000);
                    TP_Save_Adjdata();
                    LCD_Clear(WHITE);//清屏
                    return;//校正完成
            }
        }
        vTaskDelay(10);
        outtime++;
        if(outtime % 100 == 0)
        {
            sprintf(tmp,"%2dS",30-(outtime/100));
            POINT_COLOR = RED;
            LCD_ShowString(500, 400, 20, 20, 16, tmp); //显示倒计时
        }
        if(outtime > 3000)
        {
            TP_Get_Adjdata();
            break;
        }
    }
}
//触摸屏初始化
//返回值:0,没有进行校准
//       1,进行过校准

uint8_t TP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOH时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOG时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();           //开启GPIOG时钟

    //PG13 T_SCK | T_MOSI
    GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //高速
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);    //初始化

    //T_CS
    GPIO_Initure.Pin = GPIO_PIN_9;          //PF9
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //高速
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);    //初始化

    //PD3  T_PEN
    GPIO_Initure.Pin = GPIO_PIN_3;          //PD3
    GPIO_Initure.Mode = GPIO_MODE_INPUT;    //输入
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);    //初始化

    //PG12  T_MISO
    GPIO_Initure.Pin = GPIO_PIN_12;          //PG12
    GPIO_Initure.Mode = GPIO_MODE_INPUT;    //输入
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);    //初始化

    TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]); //第一次读取初始化

    return 1;
}
void Load_Drow_Dialog(void)
{
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLUE; //设置字体为蓝色
    LCD_ShowString(lcddev.width - 24, 0, 200, 16, 16, "RST"); //显示清屏区域
    POINT_COLOR = RED; //设置画笔蓝色
}
/** @brief 保存校准值
 *
 * @param
 * @param
 * @return 1 保存失败; 0  保存成功
 *
 */
uint8_t TP_Save_Adjdata(void)
{
	int temp;
    FIL fp;
    UINT bw;
    char result;
    uint8_t *p;
    cJSON *pJsonRoot = NULL;

    pJsonRoot = cJSON_CreateObject();
    if(pJsonRoot == NULL)
    {
        printf_safe("保存数据cjson创建失败!");
        return 1;
    }

    cJSON_AddNumberToObject(pJsonRoot,"is_calibrate",170);
	cJSON_AddNumberToObject(pJsonRoot, "xytype", tp_dev.touchtype);
	temp=tp_dev.xfac*100000000;
    cJSON_AddNumberToObject(pJsonRoot,"xfac",temp);
    temp=tp_dev.yfac*100000000;
    cJSON_AddNumberToObject(pJsonRoot,"yfac",temp);

    cJSON_AddNumberToObject(pJsonRoot,"xoff",tp_dev.xoff);

    cJSON_AddNumberToObject(pJsonRoot,"yoff",tp_dev.yoff);

    result = f_open(&fp, "system/CalibrationData.cfg", FA_CREATE_ALWAYS | FA_WRITE);
	//文件打开错误或者文件大于BMPMEMORYSIZE
	if(result != FR_OK)
    {
        return 1;
    }
    p = cJSON_Print(pJsonRoot);
    f_write(&fp, p, strlen(p), &bw);

    f_close(&fp);
    cJSON_Delete(pJsonRoot);
    return 0;
}
/** @brief 取得校准值
 *
 * @param
 * @param
 * @return 1: 取保存的校准值失败; 0:取保存校准值成功
 *
 */
uint8_t TP_Get_Adjdata(void)
{
    cJSON *jsCaliObj;
    ErrorCode_t errcode;
    cJSON * pSub;
    uint8_t res;
    FIL fp;
    UINT bw;
    uint8_t *p;

    errcode = ERR_NO;

    res = f_open(&fp, "system/CalibrationData.cfg", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);

    if(res == FR_OK)
    {
        if(f_size(&fp) == 0)
        {
            cJSON *pJsonRoot = NULL;

            pJsonRoot = cJSON_CreateObject();
            if(pJsonRoot == NULL)
            {
                printf_safe("保存数据cjson创建失败!");
                return 1;
            }

            cJSON_AddNumberToObject(pJsonRoot,"is_calibrate",0);
	        cJSON_AddNumberToObject(pJsonRoot, "xytype", 0);
            cJSON_AddNumberToObject(pJsonRoot,"xfac",0);
            cJSON_AddNumberToObject(pJsonRoot,"yfac",0);
            cJSON_AddNumberToObject(pJsonRoot,"xoff",0);
            cJSON_AddNumberToObject(pJsonRoot,"yoff",0);

            p = cJSON_Print(pJsonRoot);
            f_write(&fp, p, strlen(p), &bw);
            cJSON_Delete(pJsonRoot);
            free(p);
            f_close(&fp);
        }
    }

    jsCaliObj = GetCfgObj("system/CalibrationData.cfg", &errcode);

    if(jsCaliObj == NULL || errcode != ERR_NO)
    {
        return 1;
    }

    pSub = cJSON_GetObjectItem(jsCaliObj,"is_calibrate");
    if(pSub == NULL)
    {
        return 1;
    }
    if(pSub->valueint == 170)
    {
	    pSub = cJSON_GetObjectItem(jsCaliObj, "xytype");
	    tp_dev.touchtype = pSub->valueint;
	    
	    if (tp_dev.touchtype)//X,Y方向与屏幕相反
	    {
		    CMD_RDX = 0X90;
		    CMD_RDY = 0XD0;
	    }
	    else                   //X,Y方向与屏幕相同
	    {
		    CMD_RDX = 0XD0;
		    CMD_RDY = 0X90;
	    }
	    pSub = cJSON_GetObjectItem(jsCaliObj,"xfac");
        tp_dev.xfac = (float)(pSub->valueint)/100000000;

        pSub = cJSON_GetObjectItem(jsCaliObj,"yfac");
        tp_dev.yfac = (float)(pSub->valueint)/100000000;

        pSub = cJSON_GetObjectItem(jsCaliObj,"xoff");
        tp_dev.xoff = pSub->valueint;

        pSub = cJSON_GetObjectItem(jsCaliObj,"yoff");
        tp_dev.yoff = pSub->valueint;

        cJSON_Delete(jsCaliObj);
        //cJSON_Delete(pSub);
        return 0;
    }

    return 1;
}


void GUI_Touch_Calibrate()
{
    LCD_Clear(WHITE);   //ÇåÆÁ
    if(TP_Get_Adjdata() != 0)//需要校准
    {
        TP_Adjust();
    }
	
//    while(1)
//    {
//        tp_dev.scan(0);
//        if(tp_dev.sta & TP_PRES_DOWN)       //触摸屏被按下
//        {
//            if(tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)
//            {
//                if(tp_dev.x[0] > (lcddev.width - 24) && tp_dev.y[0] < 16)
//                {
//                    Load_Drow_Dialog();    //清除
//                }
//                else
//                {
//                    TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);    //画图
//                }
//            }
//        }
//        else
//        {
//             break;//vTaskDelay(10);    //没有按键按下的时候
//        }
//    }
}











