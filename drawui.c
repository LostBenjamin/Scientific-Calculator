#include <compute.h>
/********************************************************** 
 *  函数原型: void InitGraph()
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 图形方式初始化
 **********************************************************/
void InitGraph()
{
	int driver = VGA, mode = VGAHI;
	initgraph(&driver, &mode, "");
}
/********************************************************** 
 *  函数原型: void welcome()
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 绘制欢迎界面，显示作者信息、简单说明
 **********************************************************/
void welcome()
{
    int p,q;
    int pp=0,qq=0;
    int a=320, b=233;

    unsigned long gsize;

    setfillstyle(SOLID_FILL, 15);   /*绘制光标图案*/
    bar(a, b, a+MSIZEX, b+MSIZEY);     
    gsize= imagesize(a, b, a+MSIZEX, b+MSIZEY); /*保存光标图案*/
    imagep=malloc(gsize);
    imagep1=malloc(gsize);
    getimage(a, b, a+MSIZEX, b+MSIZEY, imagep);
    setfillstyle(SOLID_FILL, 0);    /*抹去光标移动痕迹*/
    bar(a, b, a+MSIZEX, b+MSIZEY);
    
    setbkcolor(0);  /*绘制欢迎界面，显示作者信息、说明信息*/
    setcolor(4);
    settextstyle(0, 0, 4);
    outtextxy(200, 30, "Welcome!");
    setcolor(15);
    settextstyle(1, 0, 3);
    outtextxy(110, 130, "It is A Scientific Calculator Made by:");
    outtextxy(125, 170, "Benjamin He, Sam Li, Leslie Rong");
    outtextxy(203, 210, "@ZheJiang University.");
    setcolor(5);
    outtextxy(70, 285, "Left Click to Enter the Scientific Calculator.");
    outtextxy(80, 320, "Please Input a Right Formula When Using.");
    outtextxy(115, 355, "Press \"ESC\" to Close the Calculator.");

    while(1)    /*处理光标移动、点击*/
    {
        MousePosition(&p,&q);   /*显示光标实时位置*/
        getimage(p,q,p+MSIZEX,q+MSIZEY,imagep1);
        putimage(p,q,imagep,COPY_PUT);
        delay(270);
        pp=p; qq=q;
        putimage(pp, qq, imagep1, COPY_PUT);    
        if(LeftPress()==1)
        {
            break;
        }
    }
}
/********************************************************** 
 *  函数原型: void UI()
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 绘制基础用户界面
 **********************************************************/
void UI()
{
    int i, j;
    int fontd= 0;

    setbkcolor(0);

    setfillstyle(SOLID_FILL, 0);    /*填涂背景*/
    bar(0, 0, 640, 640);

    setfillstyle(SOLID_FILL, 17); /*绘制显示屏上方的蓝线*/
    bar(0+BLANK, 0+BLANK, 640-BLANK, 2+BLANK);

    setfillstyle(SOLID_FILL, 8);  /*绘制显示屏*/
    bar(0+BLANK,2+BLANK, 640-BLANK, SCREENWIDTH+BLANK);

    j=0;                          /*绘制第一排亮蓝色按钮*/
    setfillstyle(SOLID_FILL, 9);
    for(i=0; i<4; i++)
    {
        if(i==2)
        {
            setfillstyle(SOLID_FILL, 3);
            bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
            setfillstyle(SOLID_FILL, 9);
        }
        else bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }

    setfillstyle(SOLID_FILL, 2); /*绘制第一排绿色按钮*/
    for(i=4; i<6; i++)
    {
        if(i==4)
        {
            setfillstyle(SOLID_FILL, 10);
            bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
            setfillstyle(SOLID_FILL, 2);
        }
        else bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }

    j=0;                          /*绘制第一排蓝色按钮*/
    setfillstyle(SOLID_FILL, 1);
    for(i=0; i<2; i++)
    {
        bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }

    setfillstyle(SOLID_FILL, 4);  /*绘制第一排红色按钮*/
    for(i=2; i<4; i++)
    {
        bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }


    setfillstyle(SOLID_FILL, 8);  /*绘制二到六排前六列按钮*/
    for(i=0; i<6; i++)
    {
        for(j=1; j<6; j++)
        {
            bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }

    for(i=0; i<4; i++)           /*绘制二到六排后四列按钮*/
    {
        for(j=1; j<6; j++)
        {
            bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }

    settextstyle(2, 0, 7);      /*绘制按钮文字标识*/
    setcolor(15);
    for(i=0; i<6; i++)      /*绘制左侧按钮文字标识*/
    {
        for(j=0; j<6; j++)
        {
            if(strlen(buttonleft[j*6+i])==0) continue;          /*设置调整字符位置的参数*/
            else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
            else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
            else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
            else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
            else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

            if(i==1&&j==4)                                          /*调整"|"字符至按钮中间*/
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);
                
            else if(i==2&&j==4)                                     /*调整"!"字符至按钮中间*/
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                
            else if(i==5&&j==4)                                     /*调整">>"字符至按钮中间*/
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                
            else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5, /*绘制剩余按钮*/
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                
        }/*for(j=0*/
    }/*for(i=0*/
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,  /*绘制取整按钮*/
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);

    for(i=0; i<4; i++)   /*绘制右侧按钮文字标识*/
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
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,   /*绘制左移按钮*/
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);

    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,   /*绘制右移按钮*/
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);


    line((6+3)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+17, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10,  /*绘制退格按钮*/
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