#include <calculator.h>

#define CHARWIDTH 12/*显示框算式每个字符的宽度*/

int th =0;/*算式长度*/
int th1=0;/*算式长度*/
int k=-1;/*back中德参数*/

char back[150][7];/*储存每次点击输入到input之中的字符串，供bakcspace调用*/

char buttonleftout[36][7]={"Bin", "Oct", "Dec", "Hex", "Rad", "Deg",            /*点击每个按钮输入到显示框的字符串*/
                           "PI",  "e",   "1/", "^2", "^", "sqrt(",
                           "sin(", "cos(", "tan(", "log(", "ln(",  "sqry(",
                           "Mod", " ",    "abs(", "!",  "Ans", "Shift",
                           "&",   "|",   "!",   "XOR", "<<",  ">>",
                           "A",   "B",   "C",   "D",   "E",   "F"};
char buttonrightout[24][7]={"",   "",  "Clear", "",
                            "(",  ")", "%",     "/",
                            "7",  "8", "9",     "*",
                            "4",  "5", "6",     "-",
                            "1",  "2", "3",     "+",
                            "00", "0", ".",     "="};
char extraout[5][7]={"asin(", "acos(", "atan(","10^","exp("};

char buttonleftstring[36][7]={"", "", "", "", "", "",                            /*点击每个按钮输入到input数组的字符串*/
                              "pi",  "e",   "1/(", "^2", "^", "sqrt(",
                              "sin(", "cos(", "tan(", "lg(", "ln(",  "^(1/",
                              "m", "",    "abs(", "!",  "", "",
                              "&",   "|",   "~",   "X", "<",  ">",
                              "A",   "B",   "C",   "D",   "E",   "F"};
char buttonrightstring[24][7]={"",   "",  "", "",
                               "(",  ")", "*0.01",     "/",
                               "7",  "8", "9",     "*",
                               "4",  "5", "6",     "-",
                               "1",  "2", "3",     "+",
                               "00", "0", ".",     "="};
char extrastring[5][7]={"asin(", "acos(", "atan(","10^(","e^("};

char binaryoperator[11]={'/', '*', '-', '+', '<', '>', 'X', '|', '&', 'm', '^'};       /*几乎所有双目运算符*/


/********************************************************** 
 *  函数原型: void outequation(int i, int j, int LR)
 *  传入参数: i-鼠标点击按钮的列数
 *            j-鼠标点击按钮的行数
              LR-鼠标点击按钮的左右位置
 *  返回值: 无
 *  函数功能: 在显示屏上输出鼠标点击按钮所代表字符串
 **********************************************************/
void outequation(int i, int j, int LR)
{
    
    if(th<50)/*将算式的前50位显示在显示框*/
    {
        if(shift==1&&LR==0&&j==2&&i<=4)/*shift键被按下*/
        {
            outtextxy(20+th*CHARWIDTH, 17, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        else if(LR==0)/*shift未被按下，被点击按钮在左边*/
        {
            outtextxy(20+th*CHARWIDTH, 17, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)/*shift未被按下，被点击按钮在右边*/
        {
            outtextxy(20+th*CHARWIDTH, 17, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=50&&th<100)/*将算式的50-100位显示在显示框*/
    {
        if(shift==1&&LR==0&&j==2&&i<=4)/*shift键被按下*/
        {
            outtextxy(20+th*CHARWIDTH, 34, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        else if(LR==0)/*shift未被按下，被点击按钮在左边*/
        {
            outtextxy(20+(th%50)*CHARWIDTH, 34, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)/*shift未被按下，被点击按钮在右边*/
        {
            outtextxy(20+(th%50)*CHARWIDTH, 34, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=100&&th<150)/*将算式的100-150位显示在显示框*/
    {
        if(shift==1&&LR==0&&j==2&&i<=4)/*shift键被按下*/
        {
            outtextxy(20+th*CHARWIDTH, 51, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        if(LR==0)/*shift未被按下，被点击按钮在左边*/
        {
            outtextxy(20+(th%50)*CHARWIDTH, 51, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)/*shift未被按下，被点击按钮在右边*/
        {
            outtextxy(20+(th%50)*CHARWIDTH, 51, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=150)/*算式长度大于150时显示错误信息*/
    {
        outtextxy(20, 68, "The formula is too long. Click \"Clear\" and restart.");
    }
}


/********************************************************** 
 *  函数原型: void backspace(void)
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 退格，在显示框上删去上一个点击按钮的字符串
 **********************************************************/
void backspace(void)
{
    
    if(k<=0) clearscreen();/*当k=0时清屏*/
    else/*画出一个back[k]长度的矩形遮住要被删去的字符*/
    {
        setfillstyle(SOLID_FILL,8);
        bar(20+(th%50)*CHARWIDTH,(th/50+1)*17,20+(th%50)*CHARWIDTH-strlen(back[k])*CHARWIDTH,(th/50+1)*17+20);
    }
    if(th>0&&th1>0)/*剪短算式长度*/
    {
        th-=strlen(back[k--]);
        th1-=strlen(back[k--]);
    }
}


/********************************************************** 
 *  函数原型: void clearscreen(void)
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 清除显示框
 **********************************************************/
void clearscreen(void)
{
    setfillstyle(SOLID_FILL, 17); /*重新绘制一条蓝线*/
    bar(0+BLANK, 0+BLANK, 640-BLANK, 2+BLANK);

    setfillstyle(SOLID_FILL, 8); /*重新绘制一个显示框*/
    bar(0+BLANK,2+BLANK, 640-BLANK, SCREENWIDTH+BLANK);

    th=0;/*算式数组长度减为0*/
    th1=0;

    memset(input, '\0', sizeof(input));/*初始化input数组*/
    memset(output, '\0', sizeof(output));/*初始化output数组*/
}


/********************************************************** 
 *  函数原型: void instring(int i, int j, int LR)
 *  传入参数: i-鼠标点击按钮的列数
 *            j-鼠标点击按钮的行数
              LR-鼠标点击按钮的左右位置
 *  返回值: 无
 *  函数功能: 将鼠标点击按钮所代表字符串储存到待计算的字符串中
 **********************************************************/
void instring(int i, int j, int LR)
{
    if(LR==0)/*按钮在左边*/
    {
        if(shift==1&&j==2)/*shift被按下*/
        {
            strcat(input, extrastring[i]);
            th1+= strlen(extrastring[i]);
            input[th1]='\0';
        }
        else if(j==3&&i==4)/*Ans功能*/
        {
            strcat(input, Ans);
            th1+= strlen(Ans);
            input[th1]=='\0';
        }
        else/*一般情况*/
        {
            strcat(input, buttonleftstring[j*6+i]);
            th1+= strlen(buttonleftstring[j*6+i]);
            input[th1]='\0';
        }
    }
    if(LR==1)/*按钮在右边*/
    {
        strcat(input, buttonrightstring[j*4+i]);
        th1+= strlen(buttonrightstring[j*4+i]);
        input[th1]='\0';
    }
}


/********************************************************** 
 *  函数原型: void outstring(void)
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 在显示框上显示计算结果
 **********************************************************/
void outstring()
{
    int p,q;
    int pp=0,qq=0;
    
    settextstyle(2, 0, 9);/*输出output字符串*/
    outtextxy(618-strlen(output)*16, 95, output);

    while(1)/*控制鼠标*/
    {
        MousePosition(&p,&q);
        getimage(p,q,p+MSIZEX,q+MSIZEY,imagep1);
        putimage(p,q,imagep,COPY_PUT);
        delay(270);
        pp=p; qq=q;
        putimage(pp, qq, imagep1, COPY_PUT);
        if(LeftPress()==1) 
        {
            clearscreen();
            show(fbuttoni(p,q),fbuttonj(p,q),fbuttonLR(p,q));
            return;
        }
    }
}

void main()
{   
    int p,q;
    int pp=0,qq=0;
    double result1;
    int key;
    
    InitMouse();
    InitGraph();
    welcome();
    UI();
    
    while(key!=0x011b)/*ESC*/
    {
        MousePosition(&p,&q);
        getimage(p,q,p+MSIZEX,q+MSIZEY,imagep1);
        putimage(p,q,imagep,COPY_PUT);
        delay(270);
        pp=p; qq=q;
        putimage(pp, qq, imagep1, COPY_PUT);
        if(LeftPress()==1)
        {
            show(fbuttoni(p,q),fbuttonj(p,q),fbuttonLR(p,q));
            if(fbuttonLR(p,q)==1&&fbuttonj(p,q)==5&&fbuttoni(p,q)==3)
            {
                result1=compute(input);
                if(fabs(result1)<1e-8) result1=0;
                sprintf(output, "%.4f", result1);/*将comoute函数返回的double类型数保留四位转入result1数组中*/
                strcpy(Ans, output);/*Ans功能*/
                outstring();
            }
        }
        if(bioskey(1)!=0) key=bioskey(0);
	}
    closegraph();
}
