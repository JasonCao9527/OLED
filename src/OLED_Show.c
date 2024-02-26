#include "OLED.h"
#include "OLED_Tools.h"

/**
  * @brief  OLED显示图像
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @param  Image 指定要显示的图像
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    uint8_t i, j;

    /*参数检查，保证指定图像不会超出屏幕范围*/
    /*最大边界值为像素总数，而不是从0开始的计数值，即128*64故此处需要减1*/
    if (X > OLED_maxwidth-1) {return;}
    if (Y > OLED_maxheight-1) {return;}

    /*将图像所在区域清空*/
    OLED_ClearArea(X, Y, Width, Height);

    /*遍历指定图像涉及的相关页*/
    /*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
    /*`-1`是为了规避8的倍数带来的计算不正确*/
    for (j = 0; j < (Height - 1) / 8 + 1; j ++)
    {
        /*遍历指定图像涉及的相关列*/
        for (i = 0; i < Width; i ++)
        {
            /*超出边界，则跳过显示*/
            /*最大边界值为像素总数，而不是从0开始的计数值，即128*64故此处需要减1*/
            if (X + i > OLED_maxwidth-1) {break;}
            if (Y / 8 + j > (OLED_maxheight/8)-1) {return;}

            /*显示图像在当前页的内容*/
            /*`%8`的作用是: todo*/
            OLED_GRAM[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);

            /*超出边界，则跳过显示*/
            /*边界设置为最大显示页数，即(OLED_maxheight/8)-1，而不是原来的7列*/
            /*使用continue的目的是，下一页超出边界时，上一页的后续内容还需要继续显示*/
//            if (Y / 8 + j + 1 > 7) {continue;}
            if (Y / 8 + j + 1 > (OLED_maxheight/8)-1) {continue;}

            /*显示图像在下一页的内容*/
            OLED_GRAM[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
        }
    }
}


/**
  * @brief  OLED显示一个字符
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Char 指定要显示的字符，范围：ASCII码可见字符
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, OLED_FontSize FontSize)
{
    if(FontSize == OLED_8X6)	//字体为宽6像素，高8像素
    {
        /*将ASCII字模库OLED_F8X6的指定数据以8*6的图像格式显示*/
        OLED_ShowImage(X, Y, 6, 8, OLED_ASCII_F8X6[Char - ' ']);
    }
    else if (FontSize == OLED_16X8)		//字体为宽8像素，高16像素
    {
        /*将ASCII字模库OLED_F16X8的指定数据以16*8的图像格式显示*/
        OLED_ShowImage(X, Y, 8, 16, OLED_ASCII_F16X8[Char - ' ']);
    }
    else if(FontSize == OLED_24X12)	//字体为宽12像素，高24像素
    {
        /*将ASCII字模库OLED_F24X12的指定数据以24*12的图像格式显示*/
        OLED_ShowImage(X, Y, 12, 24, OLED_ASCII_F24X12[Char - ' ']);//todo
    }
    else if(FontSize == OLED_32X16)	//字体为宽16像素，高32像素
    {
        /*将ASCII字模库OLED_F32X16的指定数据以32*16的图像格式显示*/
        OLED_ShowImage(X, Y, 16, 32, OLED_ASCII_F32X16[Char - ' ']);//todo
    }
    else if(FontSize == OLED_48X24)	//字体为宽24像素，高48像素
    {
        /*将ASCII字模库OLED_F48X24的指定数据以48*24的图像格式显示*/
//        OLED_ShowImage(X, Y, 24, 48, OLED_ASCII_F48X24[Char - ' ']);//todo
    }
    else if(FontSize == OLED_64X32)	//字体为宽32像素，高64像素
    {
        /*将ASCII字模库OLED_F64X32的指定数据以64*32的图像格式显示*/
//        OLED_ShowImage(X, Y, 32, 64, OLED_ASCII_F64X32[Char - ' ']);//todo
    }
}

/**
  * @brief  OLED直接显示一个字符，反白
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Char 指定要显示的字符，范围：ASCII码可见字符
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowAntiChar(uint8_t X, uint8_t Y, char Char, OLED_FontSize FontSize)
{
    if(FontSize == OLED_8X6)	//字体为宽6像素，高8像素
    {
        /*将ASCII字模库OLED_F8X6的指定数据以8*6的图像格式显示*/
        OLED_ShowImage(X, Y, 6, 8, OLED_ASCII_F8X6[Char - ' ']);
        OLED_ReverseArea(X, Y, 6, 8);
    }
    else if (FontSize == OLED_16X8)		//字体为宽8像素，高16像素
    {
        /*将ASCII字模库OLED_F16X8的指定数据以16*8的图像格式显示*/
        OLED_ShowImage(X, Y, 8, 16, OLED_ASCII_F16X8[Char - ' ']);
        OLED_ReverseArea(X, Y, 8, 16);
    }
    else if(FontSize == OLED_24X12)	//字体为宽12像素，高24像素
    {
        /*将ASCII字模库OLED_F24X12的指定数据以24*12的图像格式显示*/
        OLED_ShowImage(X, Y, 12, 24, OLED_ASCII_F24X12[Char - ' ']);//todo
        OLED_ReverseArea(X, Y, 12, 24);
    }
    else if(FontSize == OLED_32X16)	//字体为宽16像素，高32像素
    {
        /*将ASCII字模库OLED_F32X16的指定数据以32*16的图像格式显示*/
        OLED_ShowImage(X, Y, 16, 32, OLED_ASCII_F32X16[Char - ' ']);//todo
        OLED_ReverseArea(X, Y, 16, 32);
    }
    else if(FontSize == OLED_48X24)	//字体为宽24像素，高48像素
    {
        /*将ASCII字模库OLED_F48X24的指定数据以48*24的图像格式显示*/
//        OLED_ShowImage(X, Y, 24, 48, OLED_ASCII_F48X24[Char - ' ']);//todo
        OLED_ReverseArea(X, Y, 24, 48);
    }
    else if(FontSize == OLED_64X32)	//字体为宽32像素，高64像素
    {
        /*将ASCII字模库OLED_F64X32的指定数据以64*32的图像格式显示*/
//        OLED_ShowImage(X, Y, 32, 64, OLED_ASCII_F64X32[Char - ' ']);//todo
        OLED_ReverseArea(X, Y, 32, 64);
    }
}


//Ελληνικό αλφάβητο
//参照现代希腊字母表，使用latex进行编码
//Greek and Hebrew letters
//使用枚举进行选择希腊字母
/**
  * @brief  OLED直接显示Greek and Hebrew letters
  * @param  Line    起始行位置，范围：1~4
  * @param  Column  起始列位置，范围：1~16
  * @param  Letter  需要显示的Greek and Hebrew letters，按latex代码进行输入
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  */
void OLED_ShowGreekLetter(uint8_t X, uint8_t Y, GreekLetter Letter, OLED_FontSize FontSize)
{
    if(FontSize == OLED_16X16)	//字体为宽16像素，高16像素
    {
        /*将希腊字母字模库OLED_GreekLetter_F16X16的指定数据以16*16的图像格式显示*/
        OLED_ShowImage(X, Y, 16, 16, OLED_GreekLetter_F16X16[Letter]);
    }
    else if (FontSize == OLED_24X24)		//字体为宽24像素，高24像素
    {
        /*将希腊字母字模库OLED_GreekLetter_F24X24的指定数据以24*24的图像格式显示*/
//        OLED_ShowImage(X, Y, 24, 24, OLED_GreekLetter_F24X24[Letter]);
    }
    else if(FontSize == OLED_32X32)	//字体为宽32像素，高32像素
    {
        /*将ASCII字模库OLED_F32X32的指定数据以32*32的图像格式显示*/
        OLED_ShowImage(X, Y, 32, 32, OLED_GreekLetter_F32X32[Letter]);//todo
    }
}


/**
  * @brief  OLED显示字符串
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  String 指定要显示的字符串，范围：ASCII码可见字符组成的字符串
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)		//遍历字符串的每个字符
    {
        /*调用OLED_ShowChar函数，依次显示每个字符*/
        OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
    }
}


/**
  * @brief  OLED显示数字（十进制，正整数）
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Number 指定要显示的数字，范围：0~4294967295
  * @param  Length 指定数字的长度，范围：0~10
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++)		//遍历数字的每一位
    {
        /*调用OLED_ShowChar函数，依次显示每个数字*/
        /*Number / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
        /*+ '0' 可将数字转换为字符格式*/
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}

/**
  * @brief  OLED显示有符号数字（十进制，整数）
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Number 指定要显示的数字，范围：-2147483648~2147483647
  * @param  Length 指定数字的长度，范围：0~10
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    uint32_t Number1;

    if (Number >= 0)						//数字大于等于0
    {
        OLED_ShowChar(X, Y, '+', FontSize);	//显示+号
        Number1 = Number;					//Number1直接等于Number
    }
    else									//数字小于0
    {
        OLED_ShowChar(X, Y, '-', FontSize);	//显示-号
        Number1 = -Number;					//Number1等于Number取负
    }

    for (i = 0; i < Length; i++)			//遍历数字的每一位
    {
        /*调用OLED_ShowChar函数，依次显示每个数字*/
        /*Number1 / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
        /*+ '0' 可将数字转换为字符格式*/
        OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}


/**
  * @brief  OLED显示十六进制数字（十六进制，正整数）
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
  * @param  Length 指定数字的长度，范围：0~8
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)		//遍历数字的每一位
    {
        /*以十六进制提取数字的每一位*/
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;

        if (SingleNumber < 10)			//单个数字小于10
        {
            /*调用OLED_ShowChar函数，显示此数字*/
            /*+ '0' 可将数字转换为字符格式*/
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
        }
        else							//单个数字大于10
        {
            /*调用OLED_ShowChar函数，显示此数字*/
            /*+ 'A' 可将数字转换为从A开始的十六进制字符*/
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
        }
    }
}

/**
  * @brief  OLED显示二进制数字（二进制，正整数）
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
  * @param  Length 指定数字的长度，范围：0~16
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++)		//遍历数字的每一位
    {
        /*调用OLED_ShowChar函数，依次显示每个数字*/
        /*Number / OLED_Pow(2, Length - i - 1) % 2 可以二进制提取数字的每一位*/
        /*+ '0' 可将数字转换为字符格式*/
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
    }
}

/**
  * @brief  OLED显示浮点数字（十进制，小数）
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Number 指定要显示的数字，范围：-4294967295.0~4294967295.0
  * @param  IntLength 指定数字的整数位长度，范围：0~10
  * @param  FraLength 指定数字的小数位长度，范围：0~9，小数进行四舍五入显示
  * @param  FontSize 指定字体大小 范围：
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
    uint32_t PowNum, IntNum, FraNum;

    if (Number >= 0)						//数字大于等于0
    {
        OLED_ShowChar(X, Y, '+', FontSize);	//显示+号
    }
    else									//数字小于0
    {
        OLED_ShowChar(X, Y, '-', FontSize);	//显示-号
        Number = -Number;					//Number取负
    }

    /*提取整数部分和小数部分*/
    IntNum = Number;						//直接赋值给整型变量，提取整数
    Number -= IntNum;						//将Number的整数减掉，防止之后将小数乘到整数时因数过大造成错误
    PowNum = OLED_Pow(10, FraLength);		//根据指定小数的位数，确定乘数
    FraNum = round(Number * PowNum);		//将小数乘到整数，同时四舍五入，避免显示误差
    IntNum += FraNum / PowNum;				//若四舍五入造成了进位，则需要再加给整数

    /*显示整数部分*/
    OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);

    /*显示小数点*/
    OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);

    /*显示小数部分*/
    OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);
}

/**
  * @brief  OLED显示汉字串
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Chinese 指定要显示的汉字串，范围：必须全部为汉字或者全角字符，不要加入任何半角字符
  *           显示的汉字需要在OLED_Data.c里的OLED_CF16x16数组定义
  *           未找到指定汉字时，会显示默认图形（一个方框，内部一个问号）
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
    uint8_t pChinese = 0;
    uint8_t pIndex;
    uint8_t i;
    char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};

    for (i = 0; Chinese[i] != '\0'; i ++)		//遍历汉字串
    {
        SingleChinese[pChinese] = Chinese[i];	//提取汉字串数据到单个汉字数组
        pChinese ++;							//计次自增

        /*当提取次数到达OLED_CHN_CHAR_WIDTH时，即代表提取到了一个完整的汉字*/
        if (pChinese >= OLED_CHN_CHAR_WIDTH)
        {
            pChinese = 0;		//计次归零

            /*遍历整个汉字字模库，寻找匹配的汉字*/
            /*如果找到最后一个汉字（定义为空字符串），则表示汉字未在字模库定义，停止寻找*/
            for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
            {
                /*找到匹配的汉字*/
                if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
                {
                    break;		//跳出循环，此时pIndex的值为指定汉字的索引
                }
            }

            /*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
            OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
        }
    }
}



OLED_ConfigTypeDef OLED;//存放FPS的值以及FPS开关状态

// todo 将这个计时器改为没有独立IO口的`基本定时器`
void OLED_ShowFpsInit(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    //使用内部时钟
    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode= TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period= 10000-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler= 7200-1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter= 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,ENABLE);
}

void OLED_ShowFpsDisable(void)
{
    //关闭定时器时钟
}

void OLED_ShowFpsCmd(BitAction Command)
{
    if (Command==ENABLE)
    {
        OLED_ShowFpsInit();//开启定时器时钟
    }
    else
    {
        OLED_ShowFpsDisable();//关闭定时器时钟
    }
}

void OLED_ShowFps(void)
{
    OLED_ShowString(0.5*OLED_textwidth,0,"FPS:",OLED_16X16);
    OLED_ShowNum(0.8*OLED_textwidth,0,OLED.Fps,2,OLED_16X16);
}


/**
  * @brief  fpsTimer中断函数
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
    {
        OLED.Fps=OLED.FpsTemp;
        OLED.FpsTemp=0;

        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}

/**
  * @brief  OLED使用printf函数打印格式化字符串，在CLion中或不可用
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  FontSize 指定字体大小 范围：
  * @param  format 指定要显示的格式化字符串，范围：ASCII码可见字符组成的字符串
  * @param  ... 格式化字符串参数列表
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
{
    char String[30];						//定义字符数组
    va_list arg;							//定义可变参数列表数据类型的变量arg
    va_start(arg, format);					//从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);			//使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);							//结束变量arg
    OLED_ShowString(X, Y, String, FontSize);//OLED显示字符数组（字符串）
}
