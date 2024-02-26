# include "OLED.h"
#include "OLED_Tools.h"

/**
  * @brief  OLED在指定位置画一个点
  * @param  X 指定区域左上角的横坐标，范围：0~127
  * @param  Y 指定区域左上角的纵坐标，范围：0~63
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
    /*参数检查，保证指定位置不会超出屏幕范围*/
    if (X > 127) {return;}
    if (Y > 63) {return;}

    /*将显存数组指定位置的一个Bit数据置1*/
    OLED_GRAM[Y / 8][X] |= 0x01 << (Y % 8);
}

/**
  * @brief  OLED获取指定位置点的值
  * @param  X 指定点的横坐标，范围：0~127
  * @param  Y 指定点的纵坐标，范围：0~63
  * @retval 指定位置点是否处于点亮状态，1：点亮，0：熄灭
  */
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y)
{
    /*参数检查，保证指定位置不会超出屏幕范围*/
    if (X > 127) {return 0;}
    if (Y > 63) {return 0;}

    /*判断指定位置的数据*/
    if (OLED_GRAM[Y / 8][X] & 0x01 << (Y % 8))
    {
        return 1;	//为1，返回1
    }

    return 0;		//否则，返回0
}


/**
  * @brief  OLED画线
  * @param  X0 指定一个端点的横坐标，范围：0~127
  * @param  Y0 指定一个端点的纵坐标，范围：0~63
  * @param  X1 指定另一个端点的横坐标，范围：0~127
  * @param  Y1 指定另一个端点的纵坐标，范围：0~63
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;

    if (y0 == y1)		//横线单独处理
    {
        /*0号点X坐标大于1号点X坐标，则交换两点X坐标*/
        if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}

        /*遍历X坐标*/
        for (x = x0; x <= x1; x ++)
        {
            OLED_DrawPoint(x, y0);	//依次画点
        }
    }
    else if (x0 == x1)	//竖线单独处理
    {
        /*0号点Y坐标大于1号点Y坐标，则交换两点Y坐标*/
        if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}

        /*遍历Y坐标*/
        for (y = y0; y <= y1; y ++)
        {
            OLED_DrawPoint(x0, y);	//依次画点
        }
    }
    else				//斜线
    {
        /*使用Bresenham算法画直线，可以避免耗时的浮点运算，效率更高*/
        /*参考文档：https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
        /*参考教程：https://www.bilibili.com/video/BV1364y1d7Lo*/

        if (x0 > x1)	//0号点X坐标大于1号点X坐标
        {
            /*交换两点坐标*/
            /*交换后不影响画线，但是画线方向由第一、二、三、四象限变为第一、四象限*/
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }

        if (y0 > y1)	//0号点Y坐标大于1号点Y坐标
        {
            /*将Y坐标取负*/
            /*取负后影响画线，但是画线方向由第一、四象限变为第一象限*/
            y0 = -y0;
            y1 = -y1;

            /*置标志位yflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
            yflag = 1;
        }

        if (y1 - y0 > x1 - x0)	//画线斜率大于1
        {
            /*将X坐标与Y坐标互换*/
            /*互换后影响画线，但是画线方向由第一象限0~90度范围变为第一象限0~45度范围*/
            temp = x0; x0 = y0; y0 = temp;
            temp = x1; x1 = y1; y1 = temp;

            /*置标志位xyflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
            xyflag = 1;
        }

        /*以下为Bresenham算法画直线*/
        /*算法要求，画线方向必须为第一象限0~45度范围*/
        dx = x1 - x0;
        dy = y1 - y0;
        incrE = 2 * dy;
        incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0;
        y = y0;

        /*画起始点，同时判断标志位，将坐标换回来*/
        if (yflag && xyflag){OLED_DrawPoint(y, -x);}
        else if (yflag)		{OLED_DrawPoint(x, -y);}
        else if (xyflag)	{OLED_DrawPoint(y, x);}
        else				{OLED_DrawPoint(x, y);}

        while (x < x1)		//遍历X轴的每个点
        {
            x ++;
            if (d < 0)		//下一个点在当前点东方
            {
                d += incrE;
            }
            else			//下一个点在当前点东北方
            {
                y ++;
                d += incrNE;
            }

            /*画每一个点，同时判断标志位，将坐标换回来*/
            if (yflag && xyflag){OLED_DrawPoint(y, -x);}
            else if (yflag)		{OLED_DrawPoint(x, -y);}
            else if (xyflag)	{OLED_DrawPoint(y, x);}
            else				{OLED_DrawPoint(x, y);}
        }
    }
}

/**
  * @brief  OLED矩形
  * @param  X 指定矩形左上角的横坐标，范围：0~127
  * @param  Y 指定矩形左上角的纵坐标，范围：0~63
  * @param  Width 指定矩形的宽度，范围：0~128
  * @param  Height 指定矩形的高度，范围：0~64
  * @param  IsFilled 指定矩形是否填充
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
    uint8_t i, j;
    if (!IsFilled)		//指定矩形不填充
    {
        /*遍历上下X坐标，画矩形上下两条线*/
        for (i = X; i < X + Width; i ++)
        {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        /*遍历左右Y坐标，画矩形左右两条线*/
        for (i = Y; i < Y + Height; i ++)
        {
            OLED_DrawPoint(X, i);
            OLED_DrawPoint(X + Width - 1, i);
        }
    }
    else				//指定矩形填充
    {
        /*遍历X坐标*/
        for (i = X; i < X + Width; i ++)
        {
            /*遍历Y坐标*/
            for (j = Y; j < Y + Height; j ++)
            {
                /*在指定区域画点，填充满矩形*/
                OLED_DrawPoint(i, j);
            }
        }
    }
}

/**
  * @brief  OLED三角形
  * @param  X0 指定第一个端点的横坐标，范围：0~127
  * @param  Y0 指定第一个端点的纵坐标，范围：0~63
  * @param  X1 指定第二个端点的横坐标，范围：0~127
  * @param  Y1 指定第二个端点的纵坐标，范围：0~63
  * @param  X2 指定第三个端点的横坐标，范围：0~127
  * @param  Y2 指定第三个端点的纵坐标，范围：0~63
  * @param  IsFilled 指定三角形是否填充
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled)
{
    uint8_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
    uint8_t i, j;
    int16_t vx[] = {X0, X1, X2};
    int16_t vy[] = {Y0, Y1, Y2};

    if (!IsFilled)			//指定三角形不填充
    {
        /*调用画线函数，将三个点用直线连接*/
        OLED_DrawLine(X0, Y0, X1, Y1);
        OLED_DrawLine(X0, Y0, X2, Y2);
        OLED_DrawLine(X1, Y1, X2, Y2);
    }
    else					//指定三角形填充
    {
        /*找到三个点最小的X、Y坐标*/
        if (X1 < minx) {minx = X1;}
        if (X2 < minx) {minx = X2;}
        if (Y1 < miny) {miny = Y1;}
        if (Y2 < miny) {miny = Y2;}

        /*找到三个点最大的X、Y坐标*/
        if (X1 > maxx) {maxx = X1;}
        if (X2 > maxx) {maxx = X2;}
        if (Y1 > maxy) {maxy = Y1;}
        if (Y2 > maxy) {maxy = Y2;}

        /*最小最大坐标之间的矩形为可能需要填充的区域*/
        /*遍历此区域中所有的点*/
        /*遍历X坐标*/
        for (i = minx; i <= maxx; i ++)
        {
            /*遍历Y坐标*/
            for (j = miny; j <= maxy; j ++)
            {
                /*调用OLED_pnpoly，判断指定点是否在指定三角形之中*/
                /*如果在，则画点，如果不在，则不做处理*/
                if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
            }
        }
    }
}

/**
  * @brief  OLED画圆
  * @param  X 指定圆的圆心横坐标，范围：0~127
  * @param  Y 指定圆的圆心纵坐标，范围：0~63
  * @param  Radius 指定圆的半径，范围：0~255
  * @param  IsFilled 指定圆是否填充
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled)
{
    int16_t x, y, d, j;

    /*使用Bresenham算法画圆，可以避免耗时的浮点运算，效率更高*/
    /*参考文档：https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
    /*参考教程：https://www.bilibili.com/video/BV1VM4y1u7wJ*/

    d = 1 - Radius;
    x = 0;
    y = Radius;

    /*画每个八分之一圆弧的起始点*/
    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X + y, Y + x);
    OLED_DrawPoint(X - y, Y - x);

    if (IsFilled)		//指定圆填充
    {
        /*遍历起始点Y坐标*/
        for (j = -y; j < y; j ++)
        {
            /*在指定区域画点，填充部分圆*/
            OLED_DrawPoint(X, Y + j);
        }
    }

    while (x < y)		//遍历X轴的每个点
    {
        x ++;
        if (d < 0)		//下一个点在当前点东方
        {
            d += 2 * x + 1;
        }
        else			//下一个点在当前点东南方
        {
            y --;
            d += 2 * (x - y) + 1;
        }

        /*画每个八分之一圆弧的点*/
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X + y, Y + x);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - y, Y - x);
        OLED_DrawPoint(X + x, Y - y);
        OLED_DrawPoint(X + y, Y - x);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X - y, Y + x);

        if (IsFilled)	//指定圆填充
        {
            /*遍历中间部分*/
            for (j = -y; j < y; j ++)
            {
                /*在指定区域画点，填充部分圆*/
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }

            /*遍历两侧部分*/
            for (j = -x; j < x; j ++)
            {
                /*在指定区域画点，填充部分圆*/
                OLED_DrawPoint(X - y, Y + j);
                OLED_DrawPoint(X + y, Y + j);
            }
        }
    }
}

/**
  * @brief  OLED画椭圆
  * @param  X 指定圆的圆心横坐标，范围：0~127
  * @param  Y 指定圆的圆心纵坐标，范围：0~63
  * @param  A 指定椭圆的横向半轴长度，范围：0~255
  * @param  B 指定椭圆的纵向半轴长度，范围：0~255
  * @param  IsFilled 指定圆是否填充
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
    int16_t x, y, j;
    int16_t a = A, b = B;
    float d1, d2;

    /*使用Bresenham算法画椭圆，可以避免部分耗时的浮点运算，效率更高*/
    /*参考链接：https://blog.csdn.net/myf_666/article/details/128167392*/

    x = 0;
    y = b;
    d1 = b * b + a * a * (-b + 0.5);

    if (IsFilled)	//指定椭圆填充
    {
        /*遍历起始点Y坐标*/
        for (j = -y; j < y; j ++)
        {
            /*在指定区域画点，填充部分椭圆*/
            OLED_DrawPoint(X, Y + j);
            OLED_DrawPoint(X, Y + j);
        }
    }

    /*画椭圆弧的起始点*/
    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X - x, Y + y);
    OLED_DrawPoint(X + x, Y - y);

    /*画椭圆中间部分*/
    while (b * b * (x + 1) < a * a * (y - 0.5))
    {
        if (d1 <= 0)		//下一个点在当前点东方
        {
            d1 += b * b * (2 * x + 3);
        }
        else				//下一个点在当前点东南方
        {
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            y --;
        }
        x ++;

        if (IsFilled)	//指定椭圆填充
        {
            /*遍历中间部分*/
            for (j = -y; j < y; j ++)
            {
                /*在指定区域画点，填充部分椭圆*/
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }
        }

        /*画椭圆中间部分圆弧*/
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }

    /*画椭圆两侧部分*/
    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;

    while (y > 0)
    {
        if (d2 <= 0)		//下一个点在当前点东方
        {
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x ++;

        }
        else				//下一个点在当前点东南方
        {
            d2 += a * a * (-2 * y + 3);
        }
        y --;

        if (IsFilled)	//指定椭圆填充
        {
            /*遍历两侧部分*/
            for (j = -y; j < y; j ++)
            {
                /*在指定区域画点，填充部分椭圆*/
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }
        }

        /*画椭圆两侧部分圆弧*/
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }
}

/**
  * @brief  OLED画圆弧
  * @param  X 指定圆的圆心横坐标，范围：0~127
  * @param  Y 指定圆的圆心纵坐标，范围：0~63
  * @param  Radius 指定圆的半径，范围：0~255
  * @param  Y 指定圆的圆心纵坐标，范围：0~63
  * @param  StartAngle 指定圆弧的起始角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * @param  EndAngle 指定圆弧的终止角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * @param  IsFilled 指定圆是否填充
  * @retval None
  * @note   调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
    int16_t x, y, d, j;

    /*此函数借用Bresenham算法画圆的方法*/

    d = 1 - Radius;
    x = 0;
    y = Radius;

    /*在画圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
    if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
    if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
    if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
    if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}

    if (IsFilled)	//指定圆弧填充
    {
        /*遍历起始点Y坐标*/
        for (j = -y; j < y; j ++)
        {
            /*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
            if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
        }
    }

    while (x < y)		//遍历X轴的每个点
    {
        x ++;
        if (d < 0)		//下一个点在当前点东方
        {
            d += 2 * x + 1;
        }
        else			//下一个点在当前点东南方
        {
            y --;
            d += 2 * (x - y) + 1;
        }

        /*在画圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
        if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
        if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
        if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
        if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
        if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
        if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
        if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
        if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}

        if (IsFilled)	//指定圆弧填充
        {
            /*遍历中间部分*/
            for (j = -y; j < y; j ++)
            {
                /*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
                if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
                if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
            }

            /*遍历两侧部分*/
            for (j = -x; j < x; j ++)
            {
                /*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
                if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
                if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
            }
        }
    }
}

