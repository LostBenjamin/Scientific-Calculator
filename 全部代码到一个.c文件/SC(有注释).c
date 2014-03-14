#include <stdio.h>
#include <math.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>
#include <bios.h>

#define SCREENWIDTH 120
#define BUTTONY 52
#define BUTTONX  58
#define GAP 4
#define BLANK 10

#define MSIZEX 10
#define MSIZEY 15


extern int shift=0;
void far *imagep;
void far *imagep1;
extern char input[151]="", output[51]="", Ans[51]="";

extern char buttonleft[36][7]={"Bin", "Oct", "Dec", "Hex", "Rad", "Deg",
                               "PI",  "e",   "1/x", "x^2", "x^y", "sqrt",
                               "sin", "cos", "tan", "log", "ln",  "sqry",
                               "Mod", " ",    "abs", "n!",  "Ans", "Shift",
                               "&",   "|",   "!",   "XOR", "<<",  ">>",
                               "A",   "B",   "C",   "D",   "E",   "F"};
extern char buttonright[24][7]={"",   "",  "Clear", "",
                                "(",  ")", "%",     "/",
                                "7",  "8", "9",     "*",
                                "4",  "5", "6",     "-",
                                "1",  "2", "3",     "+",
                                "00", "0", ".",     "="};


extern char extra[5][7]={"asin", "acos", "atan","10^x","exp"};

void InitGraph(void);
void welcome(void);
void UI(void);

void InitMouse(void);
void MousePosition(int *x, int *y);
int LeftPress(void);
int RightPress(void);
int fbuttoni(int x,int y);
int fbuttonj(int x,int y);
int fbuttonLR(int x,int y);

void buttondown(int i, int j,int LR);
void buttonup(int i, int j,int LR);
void show(int i, int j, int LR);

void outequation(int i, int j, int LR);
void backspace(void);
void clearscreen(void);
void instring(int i, int j, int LR);
void outstring(void);
int judge(char *input);



#define MAX_TOKEN_LEN 100 /*最大长度*/
#define EXPR_INCREMENT 20 /*防止容量不够时的容量增量*/

typedef struct 
{
    double opnd; /*储存操作数*/
    char optr[11]; /*储存运算符*/
    int flag; /*区别单双目运算*/
} StackItem; /*定义一个栈项*/

typedef struct SNode 
{ 
    StackItem date;
    struct SNode * next;
} SNode, *Stack;/*定义一个栈*/

struct 
{ 
    char str[MAX_TOKEN_LEN];
    int type; 
} token;/*存储操作数或者运算符*/

struct 
{
    char *str;
    int cur;
} expr;/*存储表达式*/

Stack OPND, OPTR; /*定义全局变量,两个栈,分别存储操作数与运算符*/
int expr_size; 


/********************************************************** 
 *  函数原型: void InitStack(Stack *S)
 *  传入参数: 指向栈的指针
 *  返回值: 无
 *  函数功能: 建立一个栈
 **********************************************************/
void InitStack(Stack *S) 
{ 
    *S = (Stack)malloc(sizeof(SNode));
    if(!(*S)) 
    {
        printf("动态申请内存失败!\n");
        exit(0);
    } 
    (*S)->next = NULL;
} 

/********************************************************** 
 *  函数原型: void DestroyStack(Stack *S) 
 *  传入参数: 指向栈的指针
 *  返回值: 无
 *  函数功能: 销毁栈
 **********************************************************/
void DestroyStack(Stack *S) 
{ 
    SNode *p;
    while(p = *S) 
    {
        *S = p->next;
        free(p);
    } 
} 

/********************************************************** 
 *  函数原型: void Push(Stack S, StackItem e) 
 *  传入参数: 栈S,栈项e
 *  返回值: 无
 *  函数功能: 压栈操作
 **********************************************************/
void Push(Stack S, StackItem e) 
{ 
    SNode *p;
    p = (SNode *)malloc(sizeof(SNode));
    if(! p) 
    {
        printf("动态申请内存失败!\n");
                exit(0);
    } 
    strcpy(p->date.optr,e.optr);
    p->date.opnd = e.opnd;
    p->date.flag = e.flag;
    p->next = S->next;
    S->next = p;
}

/********************************************************** 
 *  函数原型: void Pop(Stack S,StackItem *e) 
 *  传入参数: 栈S,栈项e
 *  返回值: 无
 *  函数功能: 弹出栈项操作
 **********************************************************/
void Pop(Stack S,StackItem *e) 
{ 
    SNode *p;
    p = S->next;
    if(! p) 
    {
        printf("栈为空，不能出栈!\n");
        exit(0);
    } 
    S->next = p->next;
    strcpy(e->optr,p->date.optr);
    e->opnd = p->date.opnd;
    e->flag = p->date.flag;
    free(p);
}
/***********************************************************************************************************************************/
/********************************************************** 
 *  函数原型: void get_expr(char *s) 
 *  传入参数: 字符串s
 *  返回值: 无
 *  函数功能: 从字符串中提取表达式
 **********************************************************/
void get_expr(char *s) 
{ 
    char *p;
    int size;
    int i=0;

    expr.cur = 0;
    expr_size = 100;
    expr.str = (char*)malloc(expr_size * sizeof(char));
    if(! expr.str) 
    {
        printf("内存分配失败!\n");
        exit(0);
    }
    size = 0;
    p = expr.str;
    while((*p = s[i]) != '=') 
    {
        if(*p != ' ') 
        {
            p++;
            size++;
            if(size == expr_size-1) 
            { 
                expr_size += EXPR_INCREMENT;
                expr.str = (char*)realloc(expr.str,expr_size * sizeof(char));
                if(! expr.str) 
                {
                    printf("内存分配失败!\n");
                    exit(0);
                } 
                p = &expr.str[size]; 
            } /*size满了的时候要重新给它增加量*/
        }/*if*/
        i++; 
    } /*while*/
    *p++ = '#';
    *p = '\0';
} 

/********************************************************** 
 *  函数原型: int IsOpnd(char ch) 
 *  传入参数: 字符ch
 *  返回值: 1或0
 *  函数功能: 判断字符ch是否为操作数
 **********************************************************/
int IsOpnd(char ch) 
{ 
    if((ch >= '0') && (ch <= '9') || (ch == '.')) 
    {
        return 1;
    } 
    if((ch == '-') || (ch == '+') || (ch == '~')) 
    { 
        if((expr.cur == 0) || (expr.str[expr.cur-1] == '(')) 
        {
            return 1;
        } 
    }/*如果-,+,~前面无数字,或者是(的话,则为正负号或取反符号,也列入操作数*/
    return 0;
} 

/********************************************************** 
 *  函数原型: void gettoken() 
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 获取一个操作数或运算符
 **********************************************************/
void gettoken() 
{ 
    char *p = token.str;
    *p = expr.str[expr.cur];
    if(IsOpnd(*p)) 
    {
        while(IsOpnd(*++p = expr.str[++expr.cur])); 
        *p = '\0';
        token.type = 0;
        return;
    } /*token.type,如果是操作数则为0,是运算符为1*/

    if((*p >= 'a') && (*p <= 'z')) 
    {
        while((expr.str[expr.cur+1] >= 'a') && (expr.str[expr.cur+1] <= 'z')) 
        {
            *++p = expr.str[++expr.cur];
        }
    } 
    ++expr.cur;
    *++p = '\0';

    if(!strcmp(token.str,"e")) 
    {
        sprintf(token.str,"%.16g",exp(1));
        token.type = 0;
        return;
    } /*token.type,如果是操作数则为0,是运算符为1*/
    if(!strcmp(token.str,"pi")) 
    { 
        sprintf(token.str,"%.16g",exp(1));
        strcpy(token.str,"3.1415926535897932");
        token.type = 0;
        return;
    } /*token.type,如果是操作数则为0,是运算符为1*/

    token.type = 1; 
} 

/********************************************************** 
 *  函数原型: char Precede(StackItem *optr1, StackItem *optr2) 
 *  传入参数: 运算符1,运算符2
 *  返回值: '>'或'<'
 *  函数功能: 判断运算符优先级
 **********************************************************/
char Precede(StackItem *optr1, StackItem *optr2) 
{ 
    char *str1,*str2;
    str1 = optr1->optr;
    str2 = optr2->optr;
    if(!strcmp(str1,"ln") || !strcmp(str1,"lg") || !strcmp(str1,"sin") || !strcmp(str1,"cos") || !strcmp(str1,"tan") || 
       !strcmp(str1, "asin") || !strcmp(str1, "acos") || !strcmp(str1, "atan") || !strcmp(str1, "sqrt") || !strcmp(str1, "abs")) 
    {
        optr1->flag = 1; 
        return (!strcmp(str2,"(") || !strcmp(str2,"^") || !strcmp(str2,"!")) ? '<' : '>';
    } /*设置单目运算符优先级*/
    if(!strcmp(str1,"!")) 
    {
        optr1->flag = 1;
        return '>';
    } /*设置单目运算符优先级*/
    optr1->flag = 2;
    switch(str1[0]) 
    {
        case '|':                     return (!strcmp(str2,"|") ||
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case 'x':                     return (!strcmp(str2,"|") ||
                                              !strcmp(str2,"x") ||
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case '&':                     return (!strcmp(str2,"|") ||
                                              !strcmp(str2,"x") ||
                                              !strcmp(str2,"&") ||
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case '<': case '>':           return (!strcmp(str2,"|") ||
                                              !strcmp(str2,"x") ||
                                              !strcmp(str2,"&") ||
                                              !strcmp(str2,"<") || !strcmp(str2,">") ||
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case '+': case '-':           return (!strcmp(str2,"|") ||
                                              !strcmp(str2,"x") ||
                                              !strcmp(str2,"&") ||
                                              !strcmp(str2,"<") || !strcmp(str2,">") ||
                                              !strcmp(str2,"+") || !strcmp(str2,"-") ||
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case '*': case '/': case 'm': return (!strcmp(str2,"|") ||
                                              !strcmp(str2,"x") ||
                                              !strcmp(str2,"^") ||
                                              !strcmp(str2,"<") || !strcmp(str2,">") ||
                                              !strcmp(str2,"+") || !strcmp(str2,"-") || 
                                              !strcmp(str2,"*") || !strcmp(str2,"/") || !strcmp(str2,"m") ||                    
                                              !strcmp(str2,")") || !strcmp(str2,"#")) ? '>' : '<';
        case '(':                     return  (!strcmp(str2,")")) ? '=' : '<';
        case ')':                     return  '>';
        case '^':                     return  (!strcmp(str2,"(")|| !strcmp(str2,"!") || !strcmp(str2,"^")) ? '<' : '>';
        case '#':                     return  (!strcmp(str2,"#")) ? '=' : '<';
    }/*设置双目运算符优先级*/
} 

/********************************************************** 
 *  函数原型: long fact(long n)
 *  传入参数: n
 *  返回值: 阶乘结果
 *  函数功能: 算出阶乘
 **********************************************************/
long fact(long n) 
{ 
    return (n <= 1) ? 1 : n * fact(n-1);
}/*阶乘运算*/

/********************************************************** 
 *  函数原型: StackItem Operate(StackItem opnd1, StackItem optr, StackItem opnd2) 
 *  传入参数: 操作数1,运算符,操作数2
 *  返回值: 栈项temp
 *  函数功能: 运算操作,最后返回运算后的结果栈项
 **********************************************************/
StackItem Operate(StackItem opnd1, StackItem optr, StackItem opnd2) 
{ 
    StackItem temp;
    if(optr.flag == 1) 
    {
        if(!strcmp(optr.optr,"!")) temp.opnd = fact((long)opnd2.opnd);
        else if(!strcmp(optr.optr,"lg")) temp.opnd = log10(opnd2.opnd);
        else if(!strcmp(optr.optr,"ln")) temp.opnd = log(opnd2.opnd);
        else if(!strcmp(optr.optr,"sin")) temp.opnd = sin(opnd2.opnd);
        else if(!strcmp(optr.optr,"cos")) temp.opnd = cos(opnd2.opnd);
        else if(!strcmp(optr.optr,"tan")) temp.opnd = tan(opnd2.opnd);
        else if(!strcmp(optr.optr,"asin")) temp.opnd = asin(opnd2.opnd);
        else if(!strcmp(optr.optr,"acos")) temp.opnd = acos(opnd2.opnd);
        else if(!strcmp(optr.optr,"atan")) temp.opnd = atan(opnd2.opnd);
        else if(!strcmp(optr.optr,"sqrt")) temp.opnd = sqrt(opnd2.opnd);
        else if(!strcmp(optr.optr,"abs")) temp.opnd = fabs(opnd2.opnd);
        return temp;
    } /*单目运算符运算*/
    switch(optr.optr[0]) 
    {
        case '|': temp.opnd = (double)((int)opnd1.opnd | (int)opnd2.opnd); break;
        case 'x': temp.opnd = (double)((int)opnd1.opnd ^ (int)opnd2.opnd); break;
        case '&': temp.opnd = (double)((int)opnd1.opnd & (int)opnd2.opnd); break;
        case '<': temp.opnd = (double)((int)opnd1.opnd << (int)opnd2.opnd); break;
        case '>': temp.opnd = (double)((int)opnd1.opnd >> (int)opnd2.opnd); break;  
        case '+': temp.opnd = opnd1.opnd + opnd2.opnd; break;
        case '-': temp.opnd = opnd1.opnd - opnd2.opnd; break;
        case '*': temp.opnd = opnd1.opnd * opnd2.opnd; break;
        case '/': temp.opnd = opnd1.opnd / opnd2.opnd; break;
        case 'm': temp.opnd = (double)((int)opnd1.opnd % (int)opnd2.opnd); break;
        case '^': temp.opnd = pow(opnd1.opnd,opnd2.opnd);
    } /*双目运算符运算*/
    return temp;
}
/********************************************************** 
 *  函数原型: double compute(char *s) 
 *  传入参数: 字符串s
 *  返回值: 字符串计算结果
 *  函数功能: 计算用户输入的算式
 **********************************************************/
double compute(char *s) 
{
    StackItem optr,opnd1,opnd2;
    double result;
    
    get_expr(s);
    InitStack(&OPTR);
    InitStack(&OPND);
    strcpy(optr.optr,"#");
    Push(OPTR, optr);
    gettoken(); 
    while(strcmp(token.str,"#") || strcmp(OPTR->next->date.optr,"#")) 
    {
        if(token.type) /*如果token是运算符的话执行*/
        { 
            strcpy(optr.optr,token.str);
            switch(Precede(&(OPTR->next->date),&optr)) 
            {
                case '<': 
                    strcpy(optr.optr,token.str);
                    Push(OPTR,optr);
                    gettoken();
                    break;
                case '=': 
                    Pop(OPTR,&optr);
                    gettoken();
                    break;
                case '>': /*如果发现前面的运算符优先级比较高则先取出栈项进行计算*/
                    Pop(OPTR,&optr);
                    Pop(OPND,&opnd2);
                    if(optr.flag == 2) 
                    { 
                        Pop(OPND,&opnd1);
                    } 
                    Push(OPND,Operate(opnd1,optr,opnd2));
            } 
        } 
        else /*如果token是操作数的话执行*/
        { 
            if(token.str[0] == '~')/*如果需要取反先操作使之变为取反后的数*/
            {
                strcpy(token.str,token.str+1);
                opnd1.opnd = (double)(~(int)atof(token.str));
            }
            else        
                opnd1.opnd = atof(token.str);
            Push(OPND,opnd1);
            gettoken(); /*获取下个符号*/
        } 
    }
    result = OPND->next->date.opnd; 
    free(expr.str);
    DestroyStack(&OPTR);
    DestroyStack(&OPND);/*销毁栈和运算式*/
    return result;/*返回计算后的结果*/
} 


/**************************************************************************************************************************************/
/********************************************************** 
 *  函数原型: void InitMouse()
 *  传入参数: 无
 *  返回值: 无
 *  函数功能: 鼠标初始化
              鼠标初始化失败，程序终止退出
 **********************************************************/
void InitMouse()
{
    int *x, *y;

    union REGS r;
    r.x.ax = 0;             /*鼠标零号功能：鼠标复位*/
    int86(0x33, &r, &r);    /*调用0x33号中断*/

    if(r.x.ax != 0xFFFF)
    {
        printf("Mouse error: Not installed\n");
        exit(1);
    }

    r.x.ax = 0x0004;
    r.x.cx = 0;
    r.x.dx = 0;
    int86(0x33, &r, &r);

    r.x.ax = 2;
    int86(0x33, &r, &r);

    r.x.ax = 0x0003;
    int86(0x33, &r, &r);
    *x = r.x.cx;
    *y = r.x.dx;
}
/********************************************************** 
 *  函数原型: void MousePosition(int *x, int *y)
 *  传入参数: x,y是指针变量，指向鼠标坐标的（x，y）的地址
 *  返回值: 无
 *  函数功能: 获取鼠标坐标（x，y）
              无返回值，鼠标位置坐标由指针带回
 **********************************************************/
void MousePosition(int *x, int *y)
{
    union REGS r;
    r.x.ax = 3;             /*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);    
    *x = r.x.cx;            /*鼠标x坐标存于指针x所指的单元*/
    *y = r.x.dx;            /*鼠标y坐标存于指针y所指的单元*/
}
/********************************************************** 
 *  函数原型: int LeftPress()
 *  传入参数: 无
 *  返回值：1-鼠标左键单击；0-左键未单击
 *  函数功能: 检测鼠标左键是否单击
 **********************************************************/
int LeftPress()
{
    union REGS r;
    r.x.ax= 3;              /*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);
    
    return r.x.bx&1;        /*单击了左键函数值为1*/
}
/********************************************************** 
 *  函数原型: int RightPress()
 *  传入参数: 无
 *  返回值：1-鼠标右键单击；0-右键未单击
 *  函数功能: 检测鼠标右键是否单击
 **********************************************************/
int RightPress()
{
    union REGS r;
    r.x.ax= 3;              /*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);
    delay(30000);
    return r.x.bx&2;        /*单击了右键函数值为1*/
}
/********************************************************** 
 *  函数原型: int fbuttoni(int x,int y)
 *  传入参数: x-鼠标位置的x坐标
              y-鼠标位置的y坐标
 *  返回值：鼠标停留按钮的列数
 *  函数功能: 从鼠标停留位置坐标计算鼠标停留按钮的列数
 **********************************************************/
int fbuttoni(int x,int y){
    int i,j;
    int buttoni,buttonj,buttonLR,buttonflag;

        for(i=0;i<6;i++){
        for(j=0;j<6;j++){
        if((x>(10+i*(BUTTONX+GAP)))
        &&(x<(10+(i+1)*(BUTTONX+GAP)))
        &&(y>(140+j*(BUTTONY+GAP)))
        &&(y<(140+(j+1)*(BUTTONY+GAP)))){
                buttoni = i;
                buttonj = j;
                buttonflag = 1;
                buttonLR = 0;
                }
            }
        }
        for(i = 0;i<4;i++){
                for(j = 0;j<6;j++){
                    if((x>(i*(BUTTONX+GAP))+386)&&(x<((i+1)*(BUTTONX+GAP)+386-GAP))&&
                        (y>(j*(BUTTONY+GAP)+140))&&(y<((j+1)*(BUTTONY+GAP)+140-GAP))){
                    buttoni = i;
                    buttonj = j;
                    buttonflag = 1;
                    buttonLR = 1;
                    } 
                }
            }
    if(buttonflag==1)return buttoni;
}
/********************************************************** 
 *  函数原型: int fbuttonj(int x,int y)
 *  传入参数: x-鼠标位置的x坐标
              y-鼠标位置的y坐标
 *  返回值：鼠标停留按钮的行数
 *  函数功能: 从鼠标停留位置坐标计算鼠标停留按钮的行数
 **********************************************************/
int fbuttonj(int x,int y){
    int i,j;
    int buttoni,buttonj,buttonLR,buttonflag;
        for(i=0;i<6;i++){
        for(j=0;j<6;j++){
        if((x>(10+i*(BUTTONX+GAP)))
        &&(x<(10+(i+1)*(BUTTONX+GAP)))
        &&(y>(140+j*(BUTTONY+GAP)))
        &&(y<(140+(j+1)*(BUTTONY+GAP)))){
                buttoni = i;
                buttonj = j;
                buttonflag = 1;
                buttonLR = 0;
                }
            }
        }
        for(i = 0;i<4;i++){
                for(j = 0;j<6;j++){
                    if((x>(i*(BUTTONX+GAP))+386)&&(x<((i+1)*(BUTTONX+GAP)+386-GAP))&&
                        (y>(j*(BUTTONY+GAP)+140))&&(y<((j+1)*(BUTTONY+GAP)+140-GAP))){
                    buttoni = i;
                    buttonj = j;
                    buttonflag = 1;
                    buttonLR = 1;
                    } 
                }
            }
    if(buttonflag==1)return buttonj;
}
/********************************************************** 
 *  函数原型: int fbuttonLR(int x,int y)
 *  传入参数: x-鼠标位置的x坐标
              y-鼠标位置的y坐标
 *  返回值：LR-表示按钮的左右位置，0为左侧，1为右侧
 *  函数功能: 从鼠标停留位置坐标计算鼠标停留按钮的左右位置
 **********************************************************/
int fbuttonLR(int x,int y){
    int i,j;
    int buttoni,buttonj,buttonLR,buttonflag;
        for(i=0;i<6;i++){
        for(j=0;j<6;j++){
        if((x>(10+i*(BUTTONX+GAP)))
        &&(x<(10+(i+1)*(BUTTONX+GAP)))
        &&(y>(140+j*(BUTTONY+GAP)))
        &&(y<(140+(j+1)*(BUTTONY+GAP)))){
                buttoni = i;
                buttonj = j;
                buttonflag = 1;
                buttonLR = 0;
                }
            }
        }
        for(i = 0;i<4;i++){
                for(j = 0;j<6;j++){
                    if((x>(i*(BUTTONX+GAP))+386)&&(x<((i+1)*(BUTTONX+GAP)+386-GAP))&&
                        (y>(j*(BUTTONY+GAP)+140))&&(y<((j+1)*(BUTTONY+GAP)+140-GAP))){
                    buttoni = i;
                    buttonj = j;
                    buttonflag = 1;
                    buttonLR = 1;
                    } 
                }
            }
    if(buttonflag==1)return buttonLR;
}



/**************************************************************************************************************************************/
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
    outtextxy(80, 320, "Please Input a Right Formula When Using");
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


/**************************************************************************************************************************************/
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


/**************************************************************************************************************************************/
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
