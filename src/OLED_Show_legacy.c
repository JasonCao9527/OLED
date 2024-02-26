#include "OLED.h"
#include "OLED_Tools.h"

//#define __act_OLEDShow_legacy__ 1
//保留一部分直接显示字符的函数

#if defined __act_OLEDShow_legacy__
/**
  * @brief  OLED直接显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @note <table class="tg">
  *<thead>
  *  <tr>
  *    <th class="tg-0pky" colspan="2">行列xy映射关系</th>
  *  </tr>
  *</thead>
  *<tbody>
  *  <tr>
  *    <td class="tg-0pky">a行</td>
  *    <td class="tg-0pky">y=2*(a-1)</td>
  *  </tr>
  *  <tr>
  *    <td class="tg-0pky">b列</td>
  *    <td class="tg-0pky">x=8*(b-1)</td>
  *  </tr>
  *</tbody>
  *</table>
  * <br>同时，为了兼容ProOLED中对显存的位操作，添加了向现存同步的代码，以达到重定向的目的。
  * @retval 无
  */
void OLED_ShowCharLegacy(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(&OLED_ASCII_F16X8[Char - ' '][i],8);			//显示上半部分内容
        ProOLED_GramWriteData((Line - 1) * 2, (Column - 1) * 8+i,OLED_ASCII_F16X8[Char - ' '][i]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(&OLED_ASCII_F16X8[Char - ' '][i + 8],8);		//显示下半部分内容
        ProOLED_GramWriteData((Line - 1) * 2 + 1, (Column - 1) * 8+i,OLED_ASCII_F16X8[Char - ' '][i + 8]);
    }
}

/**
  * @brief  OLED直接显示一个字符，反白
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @note <table class="tg">
  *<thead>
  *  <tr>
  *    <th class="tg-0pky" colspan="2">行列xy映射关系</th>
  *  </tr>
  *</thead>
  *<tbody>
  *  <tr>
  *    <td class="tg-0pky">a行</td>
  *    <td class="tg-0pky">y=2*(a-1)</td>
  *  </tr>
  *  <tr>
  *    <td class="tg-0pky">b列</td>
  *    <td class="tg-0pky">x=8*(b-1)</td>
  *  </tr>
  *</tbody>
  *</table>
  * @retval 无
  */
void OLED_AntiShowCharLegacy(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_ASCII_F16X8[Char - ' '][i]^ 0xFF);			//显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2+1, (Column - 1) * 8);	//设置光标位置在下半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_ASCII_F16X8[Char - ' '][i + 8]^0xFF);		//显示下半部分内容
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
  * @retval 无
  */
void OLED_ShowGreekLetterLegacy(uint8_t Line, uint8_t Column, GreekLetter Letter)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 16);        //设置光标位置在上半部分
    OLED_WriteData(&OLED_GreekLetter_F16X16[Letter][i],16);            //显示上半部分内容

    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 16);    //设置光标位置在下半部分
    OLED_WriteData(&OLED_GreekLetter_F16X16[Letter][i + 16],16);        //显示下半部分内容

}

/**
  * @brief  OLED直接显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowStringLegacy(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}


/**
  * @brief  OLED直接显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNumLegacy(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED直接显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNumLegacy(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED直接显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNumLegacy(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
  * @brief  OLED直接显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNumLegacy(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}


/**
  * @brief  OLED浮点数转字符串，目前无法显示负数
  * @param  Number 需要转化的浮点数，范围：/
  * @param  Buffer 足够大以用于容纳转换后的字符串缓冲区，范围：/
  * @retval 无
  */
void OLED_float2string(double Number, char *Buffer) {
//    // 确保缓冲区足够大以容纳转换后的字符串
//    if (buffer == NULL) {
//        return;
//    }

    //规避负数问题
    if (Number >= 0) {

    }
    else
    {
        Number=-Number;
    }

    int16_t integer = (int16_t)Number;//强转整形去尾，获取整数部分
    int16_t decimal = Number * 1000 - integer * 1000;//获取小数部分

    // 将整数转换为字符串
    char integer_string[10];
    itoa(integer, integer_string, 10);
    // 将小数转换为字符串
    char decimal_string[10];
    itoa(decimal, decimal_string, 10);

    // 拼接和整数部分和小数部分
    strcpy(Buffer, integer_string);
    strcat(Buffer, ".");
    strcat(Buffer, decimal_string);
}

/**
  * @brief  OLED直接显示浮点数，去尾法，最多显示到第三位
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 需要转化的浮点数，范围：/
  * @retval 无
  * @attention  for循环中delay可能会出问题
  */
void OLED_ShowFloatNumLegacy(uint8_t Line, uint8_t Column, double Number)
{
    // 定义一个字符串缓冲区
    char float_buffer[32];
    //规避负数问题
    if (Number >= 0) {
        OLED_ShowChar(Line, Column, '+');
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
    }
    OLED_float2string(Number, float_buffer);

    OLED_ShowString(Line, Column+1, float_buffer);
}


#endif