#ifndef STM32F103C8TX_PROJECT_OLED_FONT_H
#define STM32F103C8TX_PROJECT_OLED_FONT_H

#include "stm32f10x.h"                  // Device header

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
    uint8_t Data[32];						//字模数据
} ChineseCell_t;

//通过枚举变量设置希腊字母表
typedef enum GreekAlphabet{
    G_Alpha,
    G_Beta,
    G_Gamma,
    G_Delta,
    G_Epsilon,
    G_Zeta,
    G_Eta,
    G_Theta,
    G_Iota,
    G_Kappa,
    G_Lambda,
    G_Mu,
    G_Nu,
    G_Xi,
    G_Omicron,
    G_Pi,
    G_Rho,
    G_Sigma,
    G_Tau,
    G_Upsilon,
    G_Phi,
    G_Chi,
    G_Psi,
    G_Omega,
    G_alpha,
    G_beta,
    G_gamma,
    G_delta,
    G_epsilon,
    G_zeta,
    G_eta,
    G_theta,
    G_iota,
    G_kappa,
    G_lambda,
    G_mu,
    G_nu,
    G_xi,
    G_omicron,
    G_pi,
    G_rho,
    G_sigma,
    G_tau,
    G_upsilon,
    G_phi,
    G_chi,
    G_psi,
    G_omega,
}GreekLetter;


/*ASCII字模数据声明*/
//请确保声明与定义的数组大小一致，否则将会发生错误。
extern const uint8_t OLED_ASCII_F8X6[][6];
extern const uint8_t OLED_ASCII_F16X8[][16];
extern const uint8_t OLED_ASCII_F24X12[][48];
extern const uint8_t OLED_ASCII_F32X16[][64];
//extern const uint8_t OLED_ASCII_F48X24[][144];
//extern const uint8_t OLED_ASCII_F64X32[][256];\\会爆FLASH


extern const uint8_t OLED_GreekLetter_F16X16[][32];
//extern const uint8_t OLED_GreekLetter_F24X24[][48];
extern const uint8_t OLED_GreekLetter_F32X32[][128];


//通过枚举变量设置OLED字体大小
typedef enum OLED_FontSizeList{
    OLED_8X6=6,
    OLED_16X8=8,
    OLED_16X16=16,
    OLED_24X12=12,
    OLED_24X24=24,
    OLED_32X16=16,
    OLED_32X32=32,
    OLED_48X24=24,
    OLED_64X32=32,
}OLED_FontSize;

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
extern const uint8_t fish_cake24X24[];
extern const uint8_t fish_cake32X32[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...


#endif //STM32F103C8TX_PROJECT_OLED_FONT_H
