#ifndef STM32F103C8TX_PROJECT_OLED_SETUP_H
#define STM32F103C8TX_PROJECT_OLED_SETUP_H

//定义你的OLED的长度像素和宽度像素
#define OLED_maxwidth 128
#define OLED_maxheight 128

#define OLED_textwidth  OLED_maxwidth
#define OLED_textheight OLED_maxheight

//进行芯片型号选择，只可以选择其中一个
//#define __SSD1306_IIC_OLED__ 1
//#define __SSD1306_SPI_OLED__ 1
//#define __SSD1306_spitest_OLED__ 1
//#define __SSD1312_IIC_OLED__ 1
//#define __SSD1312_SPI_OLED__ 1
#define __SH1107_SPI_OLED__ 1

//设置横屏或者竖屏显示 0   正向显示
//                     90  旋转90度显示
//                     180 旋转90度显示
//                     270 旋转270度显示

#define OLED_USE_HORIZONTAL 0
#define USE_HORIZONTAL 0

#endif //STM32F103C8TX_PROJECT_OLED_SETUP_H
