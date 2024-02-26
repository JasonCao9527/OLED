#include "OLED.h"
#if defined __SH1107_SPI_OLED__

/*引脚配置*********************/
//| **FUNC** | **PIN** |
//|----------|---------|
//| SCL(D0)  | B12     |
//| SDA(D1)  | B13     |
//| RES      | B15     |
//| DC       | A8      |
//| CS       | A9      |


/**
  * 函    数：OLED写D0（CLK）高低电平
  * 参    数：要写入D0的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写D0时，此函数会被调用
  *           用户需要根据参数传入的值，将D0置为高电平或者低电平
  *           当参数传入0时，置D0为低电平，当参数传入1时，置D0为高电平
  */
void OLED_W_D0(uint8_t BitValue)
{
    /*根据BitValue的值，将D0置高电平或者低电平*/
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)BitValue);
}

/**
  * 函    数：OLED写D1（MOSI）高低电平
  * 参    数：要写入D1的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写D1时，此函数会被调用
  *           用户需要根据参数传入的值，将D1置为高电平或者低电平
  *           当参数传入0时，置D1为低电平，当参数传入1时，置D1为高电平
  */
void OLED_W_D1(uint8_t BitValue)
{
    /*根据BitValue的值，将D1置高电平或者低电平*/
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)BitValue);
}

/**
  * 函    数：OLED写RES高低电平
  * 参    数：要写入RES的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写RES时，此函数会被调用
  *           用户需要根据参数传入的值，将RES置为高电平或者低电平
  *           当参数传入0时，置RES为低电平，当参数传入1时，置RES为高电平
  */
void OLED_W_RES(uint8_t BitValue)
{
    /*根据BitValue的值，将RES置高电平或者低电平*/
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)BitValue);
}

/**
  * 函    数：OLED写DC高低电平
  * 参    数：要写入DC的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写DC时，此函数会被调用
  *           用户需要根据参数传入的值，将DC置为高电平或者低电平
  *           当参数传入0时，置DC为低电平，当参数传入1时，置DC为高电平
  */
void OLED_W_DC(uint8_t BitValue)
{
    /*根据BitValue的值，将DC置高电平或者低电平*/
    GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)BitValue);
}

/**
  * 函    数：OLED写CS高低电平
  * 参    数：要写入CS的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写CS时，此函数会被调用
  *           用户需要根据参数传入的值，将CS置为高电平或者低电平
  *           当参数传入0时，置CS为低电平，当参数传入1时，置CS为高电平
  */
void OLED_W_CS(uint8_t BitValue)
{
    /*根据BitValue的值，将CS置高电平或者低电平*/
    GPIO_WriteBit(GPIOA, GPIO_Pin_9, (BitAction)BitValue);
}

/**
  * 函    数：OLED引脚初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：当上层函数需要初始化时，此函数会被调用
  *           用户需要将D0、D1、RES、DC和CS引脚初始化为推挽输出模式
  */
void OLED_GPIO_Init(void)
{
    uint32_t i, j;

    /*在初始化前，加入适量延时，待OLED供电稳定*/
    for (i = 0; i < 1000; i ++)
    {
        for (j = 0; j < 1000; j ++);
    }

    /*将D0、D1、RES、DC和CS引脚初始化为推挽输出模式*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//SCL(D0)
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//SDA(D1)
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//RES
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//DC
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//CS
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*置引脚默认电平*/
    OLED_W_D0(0);
    OLED_W_D1(1);
    OLED_W_RES(1);
    OLED_W_DC(1);
    OLED_W_CS(1);
}

/*********************引脚配置*/


/*通信协议*********************/

/**
  * 函    数：SPI发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void OLED_SPI_SendByte(uint8_t Byte)
{
    uint8_t i;

    /*循环8次，主机依次发送数据的每一位*/
    for (i = 0; i < 8; i++)
    {
        /*使用掩码的方式取出Byte的指定一位数据并写入到D1线*/
        /*两个!的作用是，让所有非零的值变为1*/
        OLED_W_D1(!!(Byte & (0x80 >> i)));
        OLED_W_D0(1);	//拉高D0，从机在D0上升沿读取SDA
        OLED_W_D0(0);	//拉低D0，主机开始发送下一位数据
    }
}

/**
  * 函    数：OLED写命令
  * 参    数：Command 要写入的命令值，范围：0x00~0xFF
  * 返 回 值：无
  */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_W_CS(0);					//拉低CS，开始通信
    OLED_W_DC(0);					//拉低DC，表示即将发送命令
    OLED_SPI_SendByte(Command);		//写入指定命令
    OLED_W_CS(1);					//拉高CS，结束通信
}

/**
  * 函    数：OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;

    OLED_W_CS(0);					//拉低CS，开始通信
    OLED_W_DC(1);					//拉高DC，表示即将发送数据
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i ++)
    {
        OLED_SPI_SendByte(Data[i]);	//依次发送Data的每一个数据
    }
    OLED_W_CS(1);					//拉高CS，结束通信
}

/*********************通信协议*/


/*硬件配置*********************/

/**
  * 函    数：OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void OLED_Init(void)
{
    OLED_GPIO_Init();			//先调用底层的端口初始化

    /*写入一系列的命令，对OLED进行初始化配置*/
    OLED_WriteCommand(0xAE);//--turn off oled panel

    OLED_WriteCommand(0x00); /*set lower column address*/
    OLED_WriteCommand(0x10); /*set higher column address*/

    OLED_WriteCommand(0xB0); /*set page address*/

    OLED_WriteCommand(0xdc); /*set display start line*/
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x81); /*contract control*/
    OLED_WriteCommand(0x6f); /*128*/
    OLED_WriteCommand(0x20); /* Set Memory addressing mode (0x20/0x21) */

    OLED_WriteCommand(0xA4); /*Disable Entire Display On (0xA4/0xA5)*/

    OLED_WriteCommand(0xA6); /*normal / reverse*/
    OLED_WriteCommand(0xA8); /*multiplex ratio*/
    OLED_WriteCommand(0x7F); /*duty = 1/64*/

    OLED_WriteCommand(0xD3); /*set display offset*/
    OLED_WriteCommand(0x00);
    if(OLED_USE_HORIZONTAL==180)
    {
        OLED_WriteCommand(0xC8);
        OLED_WriteCommand(0xA1);
    }
    else
    {
        OLED_WriteCommand(0xC0);
        OLED_WriteCommand(0xA0);
    }

    OLED_WriteCommand(0xD5); /*set osc division*/
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xD9); /*set pre-charge period*/
    OLED_WriteCommand(0x1D);

    OLED_WriteCommand(0xdb); /*set vcomh*/
    OLED_WriteCommand(0x35);

    OLED_WriteCommand(0xad); /*set charge pump enable*/
    OLED_WriteCommand(0x80); /*Set DC-DC enable (a=0:disable; a=1:enable) */
    OLED_Clear();
    OLED_WriteCommand(0xAF);

    OLED_Clear();				//清空显存数组
    OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}

#else
#endif
