/**
  ******************************************************************************
  * @file             ProOLED.c
  * @author           Eboat
  * @brief            用新思路重构OLED驱动代码，基于同步显存刷新OLED显示，添加条件编译，更加强大和兼容
  * @note             本代码基于同步显存显示，底层显示函数会被重构，所有显示均基于显存本地映像表刷新，因此刷新速度比OLED更慢
  * @attention        None
  * @date             2023/9/13
  ******************************************************************************
  */


#include "OLED.h"
#include "OLED_Tools.h"

/**
  * @brief  点反白
  * @param  None
  * @retval 无
  */
void ProOLED_GramPointInvert(uint8_t y,uint8_t x)
{
    uint8_t bx=0;
    uint8_t pos;
    pos = (y-1)/8;
    bx=(y-1)%8;
    ProOLED_GramWriteData(pos,x-1,OLED_GRAM[pos][x-1]^(1 << bx ) );

//    ProOLED_GramSync();
}

/**
  * @brief  线反白
  * @param  None
  * @retval 无
  */
void ProOLED_GramLineInvert(uint8_t y,uint8_t x,uint8_t Length, uint8_t Mode)
{
    uint8_t i;
    if(Mode == 0)
    {
        for(i = 0; i < Length; i++)
        {
            ProOLED_GramPointInvert(y,x+i);
        }
    }
    if(Mode == 1)
    {
        for(i = 0; i < Length; i++)
        {
            ProOLED_GramPointInvert(y+i,x);
        }
    }

//    ProOLED_GramSync();
}

/**
  * @brief  块反白
  * @param  None
  * @retval 无
  */
void ProOLED_GramBlockInvert(uint8_t y,uint8_t x,uint8_t BlockSize, uint8_t Length ,uint8_t Mode) {
    uint8_t i;
    if (BlockSize == 8 && Mode == 0)//向右填充
    {

    }
    if (BlockSize == 16 && Mode == 1)//向下填充
    {
        for (i = 0; i < BlockSize; i++) {
            ProOLED_GramLineInvert(y + i, x, Length, 0);
        }
        ProOLED_GramSync();
    }
}

/**
  * @brief  块滚动反白
  * @param  None
  * @retval 无
  */
void ProOLED_GramBlockRollInvert(uint8_t y,uint8_t x,uint8_t BlockSize, uint8_t Length ,uint8_t Mode)
{
    uint8_t i;
    if(BlockSize==8 && Mode==0)//向右滚动
    {

    }
    if(BlockSize==16 && Mode==1)//向下滚动
    {
        for(i=0;i<BlockSize;i++){
            ProOLED_GramLineInvert(y+i,x,Length,0);
        }
//        ProOLED_GramSync();
//        Delay_ms(500);
        for(i=0;i<BlockSize;i++){
            ProOLED_GramLineInvert(y+i,x,Length,0);
            ProOLED_GramLineInvert(y+BlockSize+i,x,Length,0);
            ProOLED_GramSync();
        }

    }
//        ProOLED_GramSync();
}

/**
  * @brief  显存全反白
  * @param  None
  * @retval 无
  */
void ProOLED_GramFullInvert(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        for(i = 0; i < 128; i++)
        {
            ProOLED_GramWriteData(j,i,OLED_GRAM[j][i]^0xFF);
        }
    }
    ProOLED_GramSync();
}


/**
  * @brief  OLED显存全局旋转
  * @param  Angle 屏幕需要旋转的角度，目前执行90度的倍数
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_Rotation(uint16_t Angle) {

}

/**
  * @brief  OLED显存局部旋转
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @param  Width 指定区域的宽度，范围：0~128
  * @param  Height 指定区域的高度，范围：0~64
  * @param  Angle 屏幕需要旋转的角度，目前执行90度的倍数
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_RotationArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint16_t Angle) {

}

/**
  * @brief  OLED显存全局镜像
  * @param  None
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_MIRROR(void) {

}