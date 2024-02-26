/**
  ******************************************************************************
  * @file             OLED.c
  * @author           江协科技（原江科大自动化协）
  * @brief            程序设计比较简单，仅用于进行快速基本的显示
  * @note             本代码基于江科大代码进行修改和添加，添加部分函数和条件编译，（同时写入显存本地映像表，兼容ProOLED，考虑可迁移性，需要考虑）
  * @attention        None
  * @date             2023/9/13
  ******************************************************************************
  */


#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "OLED_Tools.h"
//#include "OLED_Font.h"


//未来将尝试使用条件编译进行区分SPI和IIC，见ProOLED
//[C语言#if、##ifdef、#ifndef的用法详解，C语言条件编译详解](http://c.biancheng.net/view/1986.html)
//这些操作都是在预处理阶段完成的，多余的代码以及所有的宏都不会参与编译，不仅保证了代码的正确性，还减小了编译后文件的体积。
//-----------------初始化配置
#if defined __SSD1306_IIC_OLED__

#elif defined __SSD1306_SPI_OLED__
//-----------------OLED端口定义----------------

//#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//SCL
//#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)
//
//#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//SDA
//#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)
//
//#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
//#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)
//
//#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//DC
//#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_3)
//
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_WR_Byte(u8 dat,u8 cmd)
{
    u8 i;
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    for(i=0;i<8;i++)
    {
        OLED_SCL_Clr();
        if(dat&0x80)
            OLED_SDA_Set();
        else
            OLED_SDA_Clr();
        OLED_SCL_Set();
        dat<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
}

//OLED的初始化
void OLED_Init(void)
{
//    GPIO_InitTypeDef  GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 //使能A端口时钟
////    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
//    GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA
//    GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_15);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化
    OLED_SCL_Set();
    OLED_SDA_Set();
    OLED_RES_Set();
    OLED_DC_Set();
    OLED_CS_Set();
    OLED_RES_Clr();
    Delay_ms(200);
    OLED_RES_Set();

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
}

//后向兼容性
void OLED_WriteCommand(u8 dat)
{
    OLED_WR_Byte(dat,OLED_CMD);
}

void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;

    OLED_CS_Clr();					//拉低CS，开始通信
    OLED_DC_Set();					//拉高DC，表示即将发送数据
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i ++)
    {
        OLED_WR_Byte(Data[i],OLED_DATA);	//依次发送Data的每一个数据
    }
    OLED_CS_Set();					//拉高CS，结束通信
}

//void OLED_demo(void)
//{
//    u8 i,n;
//    for(i=0;i<8;i++)
//    {
//        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
//        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
//        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
//        for(n=0;n<128;n++)
//            OLED_WR_Byte(0x33,OLED_DATA);
//    }
//}


#else

#endif

uint8_t OLED_GRAM[16][128];//OLED显存本地映像

// -------------------操作命令部分
/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  * @note   OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    /*如果使用此程序驱动1.3寸的OLED显示屏，则需要解除此注释*/
    /*因为1.3寸的OLED驱动芯片（SH1106）有132列*/
    /*屏幕的起始列接在了第2列，而不是第0列*/
    /*所以需要将X加2，才能正常显示*/
    //	X += 2;

    /*通过指令设置页地址和列地址*/
    OLED_WriteCommand(0xB0 | Y);					//设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}


/**
  * @brief  开启OLED显示
  * @param  无
  * @retval 无
  */
void OLED_Display_On(void)
{
    OLED_WriteCommand(0X8D);  //SET DCDC命令
    OLED_WriteCommand(0X14);  //DCDC ON
    OLED_WriteCommand(0XAF);  //DISPLAY ON
}

/**
  * @brief  关闭OLED显示
  * @param  无
  * @retval 无
  */
void OLED_Display_Off(void)
{
    OLED_WriteCommand(0X8D);  //SET DCDC命令
    OLED_WriteCommand(0X10);  //DCDC OFF
    OLED_WriteCommand(0XAE);  //DISPLAY OFF
}


/*工具函数*********************/
/*工具函数仅供内部部分函数使用*/

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  判断指定点是否在指定多边形内部
  * @param  nvert 多边形的顶点数
  * @param  vertx verty 包含多边形顶点的x和y坐标的数组
  * @param  testx testy 测试点的X和y坐标
  * @retval 指定点是否在指定多边形内部，1：在内部，0：不在内部
  */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
    int16_t i, j, c = 0;

    /*此算法由W. Randolph Franklin提出*/
    /*参考链接：https://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        if (((verty[i] > testy) != (verty[j] > testy)) &&
            (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
        {
            c = !c;
        }
    }
    return c;
}

/**
  * @brief  判断指定点是否在指定角度内部
  * @param X Y 指定点的坐标
  * @param StartAngle EndAngle 起始角度和终止角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * @retval 指定点是否在指定角度内部，1：在内部，0：不在内部
  */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
    int16_t PointAngle;
    PointAngle = atan2(Y, X) / 3.14 * 180;	//计算指定点的弧度，并转换为角度表示
    if (StartAngle < EndAngle)	//起始角度小于终止角度的情况
    {
        /*如果指定角度在起始终止角度之间，则判定指定点在指定角度*/
        if (PointAngle >= StartAngle && PointAngle <= EndAngle)
        {
            return 1;
        }
    }
    else			//起始角度大于于终止角度的情况
    {
        /*如果指定角度大于起始角度或者小于终止角度，则判定指定点在指定角度*/
        if (PointAngle >= StartAngle || PointAngle <= EndAngle)
        {
            return 1;
        }
    }
    return 0;		//不满足以上条件，则判断判定指定点不在指定角度
}

/**
  * @brief  向显存中写数据
  * @param  Gram_Y
  * @param  Gram_X
  * @param  Data
  * @retval 无
  */
void OLED_GramWriteData(uint8_t Gram_Y, uint8_t Gram_X ,uint8_t Data)
{
    OLED_GRAM[Gram_Y][Gram_X]=Data;
}

/*********************工具函数*/


/*功能函数*********************/

/**
  * @brief  OLED显存同步
  * @param  无
  * @retval 无
  * @note   所有的显示函数，都只是对OLED显存数组进行读写
  *           随后调用OLED_Update函数或OLED_UpdateArea函数
  *           才会将显存数组的数据发送到OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_Sync(void) {
    uint8_t i, j;
    //OLED_maxheight/8来兼容不同高度的显示屏
    //OLED_maxwidth兼容不同宽度的显示屏
    for (j = 0; j < OLED_maxheight/8; j++) {
        OLED_SetCursor(j, 0);//设置光标位置在上半部分
        OLED_WriteData(OLED_GRAM[j],OLED_maxwidth);
    }
    //统计FPS
    if (OLED.FpsState==1)
    {
        OLED.FpsTemp++;
        if(OLED.Fps != 0)
        {
            OLED_ShowFps();
        }
    }
//    else//一秒后也会变正常
//    {
//        OLED.FpsTemp=0;
//    }
}


/**
  * @brief  将OLED显存数组部分更新到OLED屏幕
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @retval 无
  * @note   所有的显示函数，都只是对OLED显存数组进行读写
  *           随后调用OLED_Update函数或OLED_UpdateArea函数
  *           才会将显存数组的数据发送到OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_SyncArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t j;

    /*参数检查，保证指定区域不会超出屏幕范围*/
    if (X > OLED_maxwidth-1) {return;}
    if (Y > OLED_maxheight-1) {return;}
    if (X + Width > OLED_maxwidth) {Width = OLED_maxwidth - X;}
    if (Y + Height > OLED_maxheight) {Height = OLED_maxheight - Y;}

    /*遍历指定区域涉及的相关页*/
    /*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
    for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
    {
        /*设置光标位置为相关页的指定列*/
        OLED_SetCursor(j, X);
        /*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
        OLED_WriteData(&OLED_GRAM[j][X],Width);//等待最终确认todo
    }
    //统计FPS
    if (OLED.FpsState==1)
    {
        OLED.FpsTemp++;
    }
//    else//一秒后也会变正常
//    {
//        OLED.FpsTemp=0;
//    }
}

/**
  * @brief  OLED更新显存
  * @param  无
  * @retval 无
  * @note   兼容原本的`Sync`函数所以进行套壳
  */
void OLED_Update(void)
{
    OLED_Sync();
}

/**
  * @brief  OLED更新显存
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @retval None
  * @note   兼容原本的`SyncArea`函数所以进行套壳
  */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    OLED_SyncArea(X,Y,Width,Height);
}

/**
  * @brief  OLED清屏
  * @param  None
  * @retval None
  * @note   需要同步
  */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_maxheight/8; j++)//等待调整最大高度todo
    {
        OLED_SetCursor(j, 0);
        for(i = 0; i < OLED_maxwidth; i++)//等待调整最大宽度todo
        {
//            OLED_WriteData(0x00,1);//todo
            OLED_GramWriteData(j,i,0x00);//等待讨论
        }
    }
}

/**
  * @brief  将OLED显存数组部分清零
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t i, j;

    /*参数检查，保证指定区域不会超出屏幕范围*/
    if (X > OLED_maxwidth-1) {return;}
    if (Y > OLED_maxheight-1) {return;}
    if (X + Width > OLED_maxwidth) {Width = OLED_maxwidth - X;}
    if (Y + Height > OLED_maxheight) {Height = OLED_maxheight - Y;}

    for (j = Y; j < Y + Height; j ++)		//遍历指定页
    {
        for (i = X; i < X + Width; i ++)	//遍历指定列
        {
            OLED_GRAM[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
        }
    }
}

/**
  * @brief  将OLED显存数组全部取反
  * @param  None
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_Reverse(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_maxheight/8; j ++)				//遍历8页
    {
        for (i = 0; i < OLED_maxwidth; i ++)			//遍历128列
        {
            OLED_GRAM[j][i] ^= 0xFF;	//将显存数组数据全部取反
        }
    }
}

/**
  * @brief  将OLED显存数组部分取反
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t i, j;

    /*参数检查，保证指定区域不会超出屏幕范围*/
    if (X > OLED_maxwidth) {return;}
    if (Y > OLED_maxheight) {return;}
    if (X + Width > OLED_maxwidth) {Width = OLED_maxwidth - X;}
    if (Y + Height > OLED_maxheight) {Height = OLED_maxheight - Y;}

    for (j = Y; j < Y + Height; j ++)		//遍历指定页
    {
        for (i = X; i < X + Width; i ++)	//遍历指定列
        {
            OLED_GRAM[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
        }
    }
}

/**
  * @brief  针对OLED屏幕的demo，可以将所有函数测试
  * @param  None
  * @retval None
  * @note   本函数中测试了所有的函数，不需要使用Sync
  */
void OLED_demo(void)
{
    uint8_t k=0;
    OLED_Clear();//清屏
    OLED_Sync();
    OLED_ShowString(0,0,"|",OLED_32X16);
    OLED_ShowString(0,32,"|",OLED_32X16);
    OLED_ShowString(20,0,"OLED",OLED_32X16);
    OLED_ShowString(20,32,"Demo",OLED_16X8);
    OLED_Sync();
    Delay_ms(1000);
    OLED_Clear();
    OLED_Sync();
    OLED_ShowString(20,0,"ASCII",OLED_32X16);
    OLED_ShowString(20,32,"show",OLED_16X8);
    OLED_Sync();
    Delay_ms(1000);
    OLED_Clear();
    for (uint8_t i = 0; i < OLED_maxheight/16; ++i) {
        for (uint8_t j = 0; j < OLED_maxwidth/8; ++j) {
                OLED_ShowChar(j*8,i*16,('A'+k),OLED_16X8);
                k++;
            }
    }
    k=0;
    OLED_Sync();

    Delay_ms(1000);
    OLED_Clear();
    for (uint8_t i = 0; i < OLED_maxheight/24; ++i) {
        for (uint8_t j = 0; j < OLED_maxwidth/12; ++j) {
            OLED_ShowChar(j*12,i*24,('A'+k),OLED_24X12);
            k++;
        }
    }
    k=0;
    OLED_Sync();
    //加入一个可以让他令起一页的函数

}
