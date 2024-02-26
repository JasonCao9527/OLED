/**
  ******************************************************************************
  * @file             OLED_Tools.h
  * @author           Eboat
  * @brief            OLED工具函数的头文件，保持代码简洁性
  * @note             None
  * @attention        None
  * @date             2024/2/14
  ******************************************************************************
  */

#ifndef STM32F103C8TX_PROJECT_OLED_TOOLS_H
#define STM32F103C8TX_PROJECT_OLED_TOOLS_H

#include "stm32f10x.h"                  // Device header

//void OLED_WriteData(uint8_t Data);
void OLED_WriteData(uint8_t *Data, uint8_t Count);
void OLED_WriteCommand(uint8_t Command);
void OLED_SetCursor(uint8_t Y, uint8_t X);

uint32_t OLED_Pow(uint32_t X, uint32_t Y);
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy);
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle);
void OLED_GramWriteData(uint8_t Gram_Y, uint8_t Gram_X ,uint8_t Data);

#endif //STM32F103C8TX_PROJECT_OLED_TOOLS_H
