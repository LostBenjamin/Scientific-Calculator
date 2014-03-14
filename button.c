#include <calculator.h>

int conp=1,conq=4;  /*状态按钮（进制、角/弧度制）参数*/
/********************************************************** 
 *  函数原型: void buttondown(int i, int j,int LR)
 *  传入参数: i-该按钮所在列数
              j-该按钮所在行数
              LR-该按钮左右位置（左侧为0、右侧为1）
 *  返回值: 无
 *  函数功能: 实现按钮被按下时的图形效果
 **********************************************************/
void buttondown(int i, int j,int LR){
    int fontd= 0;
    int x,y,color;
    int flagi,flagj;
    int o;
    if (LR==0)      /*判断按钮左右位置，得出左上角坐标信息*/
    {
        x = i*(BUTTONX+GAP)+BLANK;
        y = j*(BUTTONY+GAP)+140;
    }
    else if (LR==1)
    {
        x = i*(BUTTONX+GAP)+386;
        y = j*(BUTTONY+GAP)+140;
    }

    color = 15;     /*非状态类按钮处理，绘制按下效果*/
    setfillstyle(SOLID_FILL, color);         
    if((LR==0&&j!=0)||(LR==1))
    {
        if(((LR==0)&&(i==5)&&(j==3)))   /*shift按钮处理，参数值变化*/
        {                   
            if(shift==0)
            {
                shift=1;
            }
            else if(shift==1)
            {
                shift=0;
            }

        }
        bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    else if((LR==0)&&(i<=3)&&(j==0))    /*状态类按钮处理，绘制按下效果*/
    {                    
        conp=i;
        bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    else if((LR==0)&&(i>3)&&(i<6)&&(j==0))
    {
        conq=i;
        bar(x,y,x+BUTTONX,y+BUTTONY);
    }

    flagi=i;    /*存储按钮行、列数信息*/
    flagj=j;

    settextstyle(2, 0, 7);      /*绘制按下时按钮文字标识*/
    if(!((LR==0)&&(i==5)&&(j==3)))
    {                                 
        if(shift==0)            /*shift钮未被按下的情况*/
        {
            for(i=0; i<6; i++)      /*绘制左侧按钮文字标识*/
            {
                for(j=0; j<6; j++)
                {   
                    if(LR==0&&i==flagi&&j==flagj) setcolor(0);            /*绘制按下的钮时调整文字颜色*/
                    if(strlen(buttonleft[j*6+i])==0) continue;
                    else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                    else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                    else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                    else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                    else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

                    if(i==1&&j==4)                                               /*调整"|"字符至按钮中间*/
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                    else if(i==2&&j==4)                                         /*调整"!"字符至按钮中间*/
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else if(i==5&&j==4)                                         /*调整">>"字符至按钮中间*/
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,     /*绘制剩余按钮*/
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                        setcolor(15);
                }/*for(j=0*/
            }/*for(i=0*/
        }/*if(shift==0*/

        if(shift==1)    /*shift钮已被按下的情况*/
        {
            for(i=0; i<6; i++)
            {
                for(j=0; j<6; j++)
                {   
                    if(LR==0&&i==flagi&&j==flagj)setcolor(0);                    /*绘制按下的钮时调整文字颜色*/
                    if(!((j==2)&&(i<=4)))
                    {
                        if(strlen(buttonleft[j*6+i])==0) continue;              /*设置调整字符位置的参数*/
                        else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                        else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                        else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                        else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                        else if(strlen(buttonleft[j*6+i])==5) fontd= 5;
                    }
                    else if(((j==2)&&(i<=4)))
                    {
                        if(strlen(extra[i]==3))fontd = 3;
                        else if(strlen(extra[i]==4))fontd = 4;     
                    }

                    if(i==1&&j==4)          /*调整"|"字符至按钮中间*/
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);  
                    else if(i==2&&j==4)     /*调整"!"字符至按钮中间*/
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else if(i==5&&j==4)     /*调整">>"字符至按钮中间*/                     
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else if(!((j==2)&&(i<=4)))  /*绘制不受shift影响的按钮标识*/
                    {
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                    }
                    else if((j==2)&&(i<=4))     /*绘制被shift切换的按钮标识*/
                    {
                        if(i==0)
                        {
                            outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*9,
                                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                        }
                        else if(i==1)
                        {
                            outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*20,
                                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                        }
                        else if(i==2)
                        {
                            outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*8,
                                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                        }
                        else if(i==3)
                        {
                            outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*7,
                                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                        }
                        else if(i==4)
                        {
                            outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                        }
                    }
                setcolor(15);
                }/*for(j=0*/
            }/*for(i=0*/
        }/*if(shift==1*/
        setcolor(15);
    }/*if(!((LR==0)*/
    else if(((LR==0)&&(i==5)&&(j==3)))  /*绘制被按下的shift按钮文字标识*/
    {
        fontd=5;
        setcolor(0);
        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
        setcolor(15);
    }

    if(LR==0&&flagi==1&&flagj==3) setcolor(0);  /*绘制被按下的取整按钮标识*/
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    setcolor(15);

    for(i=0; i<4; i++)      /*绘制右侧按钮文字标识*/
    {
        for(j=0; j<6; j++)
        {
            if(!((j==0)&&((i<=1)||(i==3))))
            {
                if(LR==1&&i==flagi&&j==flagj) setcolor(0);      /*绘制按下的钮时调整文字颜色*/
            }
            if(strlen(buttonright[j*4+i])==0) continue;         /*设置调整字符位置的参数*/
            else if(strlen(buttonright[j*4+i])==1) fontd= 1;
            else if(strlen(buttonright[j*4+i])==2) fontd= 2;
            else if(strlen(buttonright[j*4+i])==3) fontd= 3;
            else if(strlen(buttonright[j*4+i])==4) fontd= 4;
            else if(strlen(buttonright[j*4+i])==5) fontd= 5;
            outtextxy((6+i)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-fontd*5,
            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonright[j*4+i]);
            setcolor(15);
        }
    }

    if(LR==1&&flagi==0&&flagj==0) setcolor(0);      /*绘制被按下的左移按钮标识*/
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);
    setcolor(15);

    if(LR==1&&flagi==1&&flagj==0) setcolor(0);      /*绘制被按下的左移按钮标识*/
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);
    setcolor(15);

    if(LR==1&&flagi==3&&flagj==0)setcolor(0);       /*绘制被按下的退格按钮标识*/
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-5,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-5,
            (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+5);
    setcolor(15);
}

/********************************************************** 
 *  函数原型: void buttonup(int i, int j,int LR)
 *  传入参数: i-该按钮所在列数
              j-该按钮所在行数
              LR-该按钮左右位置（左侧为0、右侧为1）
 *  返回值: 无
 *  函数功能: 实现按钮弹起时的图形效果
 **********************************************************/
void buttonup(int i, int j,int LR){
    int x,y,color;
    int o;
    int fontd=0;
    color = 8;

    if(LR==0&&j==0&&(i>=0)&&(i<=3)) color = 9;      /*配置不同按钮弹起时的颜色*/
    else if(LR==0&&j==0&&(i>=4)&&(i<=5)) color = 2;
    else if(LR==1&&j==0&&(i>=0)&&(i<=1)) color = 1;
    else if(LR==1&&j==0&&(i>=2)&&(i<=3)) color = 4;

    if (LR==0)      /*判断按钮左右位置，得出左上角坐标信息*/
    {
        x = i*(BUTTONX+GAP)+BLANK;
        y = j*(BUTTONY+GAP)+140;
    }
    else if (LR==1)
    {
        x = i*(BUTTONX+GAP)+386;
        y = j*(BUTTONY+GAP)+140;
    }
    setfillstyle(SOLID_FILL, color);    /*非状态类按钮处理，绘制弹起效果*/
    if(((LR==0)&&(j!=0))||(LR==1))
    {
        if(!((LR==0)&&(i==5)&&(j==3))&&(!((LR==0)&&(j==2)&&(i<=4)))) bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    else if((i<=3)&&(j==0))              /*状态类按钮（进制转换）处理，绘制弹起效果*/
    {                                    
        setfillstyle(SOLID_FILL, 3);
        bar(i*(BUTTONX+GAP)+BLANK,j*(BUTTONY+GAP)+140,i*(BUTTONX+GAP)+BLANK+BUTTONX,j*(BUTTONY+GAP)+140+BUTTONY);
        setfillstyle(SOLID_FILL, 9);
        for(o=0; o<4; o++)
        {
            if(o==i) continue;
            bar(o*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (o+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }
    else if((i>3)&&(i<6)&&(j==0))       /*状态类按钮（角/弧度制转换）处理，绘制弹起效果*/
    {                              
        setfillstyle(SOLID_FILL, 10);
        bar(i*(BUTTONX+GAP)+BLANK,j*(BUTTONY+GAP)+140,i*(BUTTONX+GAP)+BLANK+BUTTONX,j*(BUTTONY+GAP)+140+BUTTONY);
        setfillstyle(SOLID_FILL, 2);
        for(o=4; o<6; o++)
        {   
            if(o==i) continue;
            bar(o*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (o+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }

    if(shift==1) setfillstyle(SOLID_FILL, 3);       /*设置shift按钮不同状态时的被切换按钮颜色*/
    else if(shift==0) setfillstyle(SOLID_FILL, 8);

    for(o=0; o<5; o++)                              /*绘制shift按钮不同状态时的被切换按钮*/
    {
        bar(o*(BUTTONX+GAP)+BLANK, SCREENWIDTH+2*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (o+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(2+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }
    bar(5*(BUTTONX+GAP)+BLANK, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK,
        (5+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(3+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    settextstyle(2, 0, 7);

    if(shift==0)                /*绘制shift未按下时的按钮文字标识*/
    {   
        for(i=0; i<6; i++)      /*绘制左侧按钮的文字标识*/
        {
            for(j=0; j<6; j++)
            {
                if(strlen(buttonleft[j*6+i])==0) continue;
                else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

                if(i==1&&j==4)                                              /*调整"|"字符至按钮中间*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                else if(i==2&&j==4)                                         /*调整"!"字符至按钮中间*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(i==5&&j==4)                                         /*调整">>"字符至按钮中间*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,     /*绘制剩余按钮*/
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
            }/*for(j=0*/
        }/*for(i=0*/
    }/*if(shift==0*/

    if(shift==1)        /*绘制shift未按下时的按钮文字标识*/
    {
        for(i=0; i<6; i++)      /*绘制左侧按钮的文字标识*/
        {
            for(j=0; j<6; j++)
            {   
                if(!((j==2)&&(i<=4)))       /*不受shift影响的按钮参数调整*/
                {
                    if(strlen(buttonleft[j*6+i])==0) continue;
                    else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                    else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                    else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                    else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                    else if(strlen(buttonleft[j*6+i])==5) fontd= 5;
                }
                else if(((j==2)&&(i<=4)))   /*被shift切换的按钮参数调整*/
                {
                    if(strlen(extra[i]==3)) fontd = 3;
                    else if(strlen(extra[i]==4)) fontd = 4;
                }

                if(i==1&&j==4)                              /*调整"|"字符至按钮中间*/                                   
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                else if(i==2&&j==4)                         /*调整"!"字符至按钮中间*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(i==5&&j==4)                         /*调整">>"字符至按钮中间*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(!((j==2)&&(i<=4)))                  /*绘制不受shift影响的按钮弹起效果*/
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                else if((j==2)&&(i<=4))                     /*绘制被shift切换的按钮弹起效果*/
                {
                    if(i==0)
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*9,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                    else if(i==1)
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*20,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                    else if(i==2)
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*8,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                    else if(i==3)
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*7,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                    else if(i==4)
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, extra[i]);
                }    
            }/*for(j=0*/
        }/*for(i=0*/
    }/*if(shift==0*/

    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,      /*绘制取整按钮*/
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);

    for(i=0; i<4; i++)      /*绘制右侧按钮文字标识*/
    {
        for(j=0; j<6; j++)
        {
            if(strlen(buttonright[j*4+i])==0) continue;
            else if(strlen(buttonright[j*4+i])==1) fontd= 1;
            else if(strlen(buttonright[j*4+i])==2) fontd= 2;
            else if(strlen(buttonright[j*4+i])==3) fontd= 3;
            else if(strlen(buttonright[j*4+i])==4) fontd= 4;
            else if(strlen(buttonright[j*4+i])==5) fontd= 5;
            outtextxy((6+i)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-fontd*5,
                SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonright[j*4+i]);
        }
    }
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,      /*绘制左移按钮*/
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);

    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,      /*绘制右移按钮*/
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);


    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10,        /*绘制退格按钮*/
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5-15, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-5,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-5,
        (6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-5+2, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+5);

}
/********************************************************** 
 *  函数原型: void show(int i, int j, int LR)
 *  传入参数: i-该按钮所在列数
              j-该按钮所在行数
              LR-该按钮左右位置（左侧为0、右侧为1）
 *  返回值: 无
 *  函数功能: 调用函数实现按键按下-延时-弹起效果以及部分按钮功能，
              传入按键信息
 **********************************************************/
void show(int i, int j, int LR)
{
    void instring(int i, int j, int LR);

    buttondown( i,  j,  LR);        /*实现按键按下-延时-弹起效果*/
    delay(1000);
    buttonup(i, j, LR);

    if(LR==0&&j==3&&i==5);          
    else if(j!=0) outequation(i, j ,LR);
    else if(LR==1&&j==0&&i==2) clearscreen();
    else if(LR==1&&j==0&&i==3) backspace();
    instring(i, j, LR);
}