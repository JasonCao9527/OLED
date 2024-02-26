# OLED简单库函数

本函数库基于[江协科技](https://space.bilibili.com/383400717)提供的[OLED库函数](https://jiangxiekeji.com/tutorial/oled.html)进行构建，初始化部分融入[中景圆电子](http://www.zjydisplay.com/)提供的示例代码，添加了几个自己编写的函数。

向[江协科技](https://space.bilibili.com/383400717)致敬。

## OLED显示原理
OLED的显存实际上就是一根根的小棍棍，对其进行一系列位操作和取反、读取等操作。

## 初始化部分（Setup）
- OLEDsetup参照u8g2进行屏幕的初始化
- 适应不同像素尺寸屏幕
- 软件/硬件SPI的驱动代码（todo）
- 加入DMA（todo）
- `demo`函数，可以测试所有的函数

demo函数流程：
* 显示OLED测试，加以动画
* 依次展示ASCII字库，8x16，16x8，24x12，32x16
* 以此展示希腊字母16x16，24x24，32x32
* 多种大小同屏显示
* 显示icon

## 显示部分（Show）
- `OLED_ShowImage`显示图片;基础函数
- `OLED_ShowChar`显示ASCII可见字符
- `OLED_ShowGreekLetter`显示希腊字母
- `OLED_ShowString`显示ASCII可见字符串
- `OLED_ShowNum`显示数字（十进制，正整数）
- `OLED_ShowSignedNum`显示有符号数字（十进制，整数）
- `OLED_ShowHexNum`显示十六进制数字（十六进制，正整数）
- `OLED_ShowBinNum`显示二进制数字（二进制，正整数）
- `OLED_ShowFloatNum`显示浮点数字（十进制，小数）
- `OLED_ShowChinese`显示汉字串
- `OLED_Printf`使用printf函数打印格式化字符串（CLion中不可用）todo
- `OLED_ShowFps`显示屏幕全局刷新率（使用基本定时器计时）todo

## 画图部分（Draw）
- `OLED_DrawPoint`在指定位置画一个点;基础函数
- `OLED_GetPoint`获取指定位置点的值
- `OLED_DrawLine`使用Bresenham算法画直线
- `OLED_DrawRectangle`OLED画直线
- OLED多点多边形（todo）
- `OLED_DrawTriangle`OLED画三角形
- `OLED_DrawCircle`使用Bresenham算法画圆
- `OLED_DrawEllipse`使用Bresenham算法画椭圆
- `OLED_DrawArc`OLED画圆弧
- 引入厚度概念（todo）

## 3维画图部分 Cart_Draw
- 移植库函数（todo）

移植[星辰大海MFA](https://www.bilibili.com/video/BV1U2421F7Sr)分享的绘制立体图形的代码。

## plot
- 坐标系绘制（todo）
- 根据点集画线（todo）

## plot3
todo

## 工具函数
- 片上的编码转换（讨论）

## 显存操作部分
- OLED显存全局旋转（todo）
- OLED显存局部旋转（todo）
- OLED显存全局镜像（todo）
- 图像移动（todo）
- 图像对称函数（todo）
- 图像旋转算法移植（todo）
- 加入测点唯一坐标（todo）
- 解算点唯一坐标（todo）
- 取对称点坐标(数学象限)（todo）
- 等距旋转取点返回点集（todo）
- 圆域旋转（讨论）
- 屏幕旋转特定角度（todo）
- 屏幕滚动，使用循环队列或使用双向指针（todo）
- 显示方向调节（讨论使用显存操作还是直接使用芯片指令）




## 拓展
- 使用外部字库显示中文，等待w25进行驱动且需要解决中文编码（todo）
- 学习TFT_espi，加入对字体的支持？（可行性等待讨论）
- 加入了几个计算机图形学函数（todo）


## 片上字库
- 片上字库的字模名字标记为为高\*宽，但是在底层的`OLED_ShowImage`函数中是填入宽再填入高，需要注意
- ASCII字库为宋体
- 希腊字母字库为Times
- 字体大小为8\*6、16\*8、32\*16（注：希腊字母最小为16\*16）
- 字库取模字体需要注意<mark>嵌入式设备字体的版权问题</mark>

### ascii全文
```markdown
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
```
注意ascii第一个字是空格


### 希腊字母全文与对照表
```markdown
ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ
αβγδεζηθικλμνξοπρστυφχψω
```

| **Letter** | **Code** |
|------------|----------|
| Α          | \Alpha   |
| Β          | \Beta    |
| Γ          | \Gamma   |
| Δ          | \Delta   |
| Ε          | \Epsilon |
| Ζ          | \Zeta    |
| Η          | \Eta     |
| Θ          | \Theta   |
| Ι          | \Iota    |
| Κ          | \Kappa   |
| Λ          | \Lambda  |
| Μ          | \Mu      |
| Ν          | \Nu      |
| Ξ          | \Xi      |
| Ο          | \Omicron |
| Π          | \Pi      |
| Ρ          | \Rho     |
| Σ          | \Sigma   |
| Τ          | \Tau     |
| Υ          | \Upsilon |
| Φ          | \Phi     |
| Χ          | \Chi     |
| Ψ          | \Psi     |
| Ω          | \Omega   |
| α          | \alpha   |
| β          | \beta    |
| γ          | \gamma   |
| δ          | \delta   |
| ε          | \epsilon |
| ζ          | \zeta    |
| η          | \eta     |
| θ          | \theta   |
| ι          | \iota    |
| κ          | \kappa   |
| λ          | \lambda  |
| μ          | \mu      |
| ν          | \nu      |
| ξ          | \xi      |
| ο          | \omicron |
| π          | \pi      |
| ρ          | \rho     |
| σ          | \sigma   |
| τ          | \tau     |
| υ          | \upsilon |
| φ          | \phi     |
| χ          | \chi     |
| ψ          | \psi     |
| ω          | \omega   |


实际使用中应将对应代码的`\xx`更改为`G_xx`，例如Δ的代码是`\Delta`但是在输入时，应该输入`G_Delta`

## OLED取模教程
todo


## LICENSE
部分驱动代码、显示部分、绘图部分由江科大的开源程序移植而来。
```html
/***************************************************************************************
  * 本程序由江协科技创建并免费开源共享
  * 你可以任意查看、使用和修改，并应用到自己的项目之中
  * 程序版权归江协科技所有，任何人或组织不得将其据为己有
  * 
  * 程序名称：				0.96寸OLED显示屏驱动程序（7针脚SPI接口）
  * 程序创建时间：			2023.10.24
  * 当前程序版本：			V1.1
  * 当前版本发布时间：		2023.12.8
  * 
  * 江协科技官方网站：		jiangxiekeji.com
  * 江协科技官方淘宝店：	jiangxiekeji.taobao.com
  * 程序介绍及更新动态：	jiangxiekeji.com/tutorial/oled.html
  * 
  * 如果你发现程序中的漏洞或者笔误，可通过邮件向我们反馈：feedback@jiangxiekeji.com
  * 发送邮件之前，你可以先到更新动态页面查看最新程序，如果此问题已经修改，则无需再发邮件
  ***************************************************************************************
  */
```
3D绘画部分由[星辰大海MFA](https://www.bilibili.com/video/BV1U2421F7Sr)分享的绘制立体图形的代码移植。

本代码开源协议为MIT。

---

- 本项目只是笔者自用的项目驱动OLED所用的库，并非专业库
- 由于水平有限，很多用能实现不够完美，请海涵
- 笔者并**不是**专业的计算机图形学学者，仅仅是一名自学的爱好者，所作的代码仅供参考
