#include "OLED.h"

#if defined __SSD1312_IIC_OLED__

//端口定义
#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//DIN
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

//res只有7改4才需要使用
//#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
//#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//设置显示方向
#define USE_HORIZONTAL 90

#if USE_HORIZONTAL==0||USE_HORIZONTAL==180 //竖屏显示
#define oled_x  64
#define oled_y  16
#else                                      //横屏显示
#define oled_x  128
#define oled_y  8
#endif

//延时
void IIC_delay(void)
{
    u8 t=3;
    while(t--);
}

//起始信号
void I2C_Start(void)
{
    OLED_SDA_Set();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SDA_Clr();
    IIC_delay();
    OLED_SCL_Clr();
    IIC_delay();
}

//结束信号
void I2C_Stop(void)
{
    OLED_SDA_Clr();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SDA_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
    OLED_SDA_Set();
    IIC_delay();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SCL_Clr();
    IIC_delay();
}


//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			OLED_SDA_Set();
    }
		else
		{
			OLED_SDA_Clr();
    }
		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();//将时钟信号设置为低电平
		dat<<=1;
  }
}

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


//OLED的初始化
void OLED_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化PA0,1
// 	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //7针改4针的时候使用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化PA2
 	GPIO_SetBits(GPIOA,GPIO_Pin_2);

//	OLED_RES_Clr();//7针改4针的时候使用
	Delay_ms(200);
//	OLED_RES_Set();//7针改4针的时候使用

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel

	OLED_WR_Byte(0x00,OLED_CMD);    /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD);    /*set higher column address*/

	OLED_WR_Byte(0xB0,OLED_CMD);    /*set page address*/

	OLED_WR_Byte(0x81,OLED_CMD);    /*contract control*/
	OLED_WR_Byte(0x5f,OLED_CMD);    /*128*/

    //设置显示方向，显示方向不等于屏幕方向
	if(USE_HORIZONTAL==0)
	{
		OLED_WR_Byte(0x20,OLED_CMD);    /* Set Memory addressing mode (0x20/0x21) */
		OLED_WR_Byte(0x09,OLED_CMD);     /* 0x09 */

		OLED_WR_Byte(0xA1,OLED_CMD);    /*set segment remap  0XA1 */
		OLED_WR_Byte(0xC8,OLED_CMD);    /*Com scan direction   0Xc8  */
	}
	else if(USE_HORIZONTAL==90)
	{
		OLED_WR_Byte(0x20,OLED_CMD);    /* Set Memory addressing mode (0x20/0x21) */
		OLED_WR_Byte(0x02,OLED_CMD);     /* 0x02 */

		OLED_WR_Byte(0xA1,OLED_CMD);    /*set segment remap  0XA1 */
		OLED_WR_Byte(0xC0,OLED_CMD);    /*Com scan direction   0Xc0  */
	}
	else if(USE_HORIZONTAL==180)
	{
		OLED_WR_Byte(0x20,OLED_CMD);    /* Set Memory addressing mode (0x20/0x21) */
		OLED_WR_Byte(0x09,OLED_CMD);     /* 0x09 */

		OLED_WR_Byte(0xA0,OLED_CMD);    /*set segment remap  0XA0 */
		OLED_WR_Byte(0xC0,OLED_CMD);    /*Com scan direction   0Xc0  */
	}
	else if(USE_HORIZONTAL==270)
	{
		OLED_WR_Byte(0x20,OLED_CMD);    /* Set Memory addressing mode (0x20/0x21) */
		OLED_WR_Byte(0x02,OLED_CMD);     /* 0x02 */

		OLED_WR_Byte(0xA0,OLED_CMD);    /*set segment remap  0XA0 */
		OLED_WR_Byte(0xC8,OLED_CMD);    /*Com scan direction   0Xc8  */
	}

	OLED_WR_Byte(0xA4,OLED_CMD);    /*Disable Entire Display On (0xA4/0xA5)*/

	OLED_WR_Byte(0xA6,OLED_CMD);    /*normal / reverse*/

	OLED_WR_Byte(0xA8,OLED_CMD);    /*multiplex ratio*/
	OLED_WR_Byte(0x3F,OLED_CMD);    /*duty = 1/64*/

	OLED_WR_Byte(0xD3,OLED_CMD);    /*set display offset*/
	OLED_WR_Byte(0x00,OLED_CMD);    /*   0x20   */

	OLED_WR_Byte(0xD5,OLED_CMD);    /*set osc division*/
	OLED_WR_Byte(0x80,OLED_CMD);

	OLED_WR_Byte(0xD9,OLED_CMD);    /*set pre-charge period*/
	OLED_WR_Byte(0x22,OLED_CMD);

	OLED_WR_Byte(0xDA,OLED_CMD);    /* Set SEG Pins Hardware Configuration */
	OLED_WR_Byte(0x10,OLED_CMD);

	OLED_WR_Byte(0xdb,OLED_CMD);    /*set vcomh*/
	OLED_WR_Byte(0x30,OLED_CMD);

	OLED_WR_Byte(0x8d,OLED_CMD);    /*set charge pump enable*/
	OLED_WR_Byte(0x72,OLED_CMD);    /* 0x12:7.5V; 0x52:8V;  0x72:9V;  0x92:10V */
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD);
}

//后向兼容性
void OLED_WriteCommand(u8 dat)
{
    OLED_WR_Byte(dat,OLED_CMD);
}

//todo
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    for (int i = 0; i < Count; ++i) {
        OLED_WR_Byte(Data[i],OLED_DATA);
    }
}


#else
#endif