#ifndef STM32F103C8TX_PROJECT_OLED_H
#define STM32F103C8TX_PROJECT_OLED_H
#include "stm32f10x.h"                  // Device header
#include "OLED_Data.h"
#include <stdlib.h>
#include <string.h>
#include "Delay.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

#include "OLED_Setup.h"

/*参数定义*/
extern uint8_t OLED_GRAM[16][128];/*显存声明，根据实际屏幕进行修改*/

extern uint8_t ProOLED_fps;
extern uint8_t ProOLED_fpsTemp;

typedef struct
{
    uint8_t Fps;
    uint8_t FpsTemp;
    FunctionalState FpsState;
}OLED_ConfigTypeDef;

extern OLED_ConfigTypeDef OLED;

//通过枚举变量设置OLED字体大小
typedef enum OLED_DrawIsFilled{
    OLED_UNFILLED=0,
    OLED_FILLED=1,
}IsFilledModeSet;


/*函数声明*********************/
//通信协议函数



//初始化函数
void OLED_Init(void);

/*更新函数*/
void OLED_Sync(void);
void OLED_SyncArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
void OLED_Update(void);
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);


/*显示函数*/
//void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize);
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, OLED_FontSize FontSize);
void OLED_ShowAntiChar(uint8_t X, uint8_t Y, char Char, OLED_FontSize FontSize);
void OLED_ShowGreekLetter(uint8_t X, uint8_t Y, GreekLetter Letter, OLED_FontSize FontSize);
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese);
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_ShowFps(void);
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...);




//legacy
//void OLED_AntiShowChar(uint8_t Line, uint8_t Column, char Char);
//void OLED_ShowCharTest(uint8_t Line, uint8_t Column, char Char);
//void OLED_ShowGreekLetter(uint8_t Line, uint8_t Column, GreekLetter Letter);

//void OLED_ShowPictrue(uint8_t Line, uint8_t Column);
//void OLED_ShowFullPictrue(void);



//画图函数
void OLED_DrawPoint(uint8_t X, uint8_t Y);
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y);
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);



//***

void ProOLED_GramWriteData(uint8_t Gram_Y, uint8_t Gram_X ,uint8_t Data);
void ProOLED_GramSync(void);

void ProOLED_GramPointInvert(uint8_t y,uint8_t x);
void ProOLED_GramLineInvert(uint8_t y,uint8_t x,uint8_t Length, uint8_t Mode);
void ProOLED_GramBlockInvert(uint8_t y,uint8_t x,uint8_t BlockSize, uint8_t Length ,uint8_t Mode);
void ProOLED_GramBlockRollInvert(uint8_t y,uint8_t x,uint8_t BlockSize, uint8_t Length ,uint8_t Mode);


void ProOLED_GramFullInvert(void);


void ProOLED_fpsTimer_Init(void);

void OLED_demo(void);


#endif //STM32F103C8TX_PROJECT_OLED_H
