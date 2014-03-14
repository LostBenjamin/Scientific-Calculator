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



#define MAX_TOKEN_LEN 100 
#define EXPR_INCREMENT 20

typedef struct 
{
    double opnd;
    char optr[11]; 
    int flag; 
} StackItem; 

typedef struct SNode 
{ 
    StackItem date;
    struct SNode * next;
} SNode, *Stack;

struct 
{ 
    char str[MAX_TOKEN_LEN];
    int type; 
} token;

struct 
{
    char *str;
    int cur;
} expr;

Stack OPND, OPTR; 
int expr_size; 

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

void DestroyStack(Stack *S) 
{ 
    SNode *p;
    while(p = *S) 
    {
        *S = p->next;
        free(p);
    } 
} 

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
            } 
        }
        i++; 
    } 
    *p++ = '#';
    *p = '\0';
} 

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
    }
    return 0;
} 

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
    } 

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
    } 
    if(!strcmp(token.str,"pi")) 
    { 
        sprintf(token.str,"%.16g",exp(1));
        strcpy(token.str,"3.1415926535897932");
        token.type = 0;
        return;
    } 
    token.type = 1; 
} 

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
    } 
    if(!strcmp(str1,"!")) 
    {
        optr1->flag = 1;
        return '>';
    } 
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
    }
} 

long fact(long n) 
{ 
    return (n <= 1) ? 1 : n * fact(n-1);
}

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
    } 
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
    } 
    return temp;
}

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
        if(token.type) 
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
                case '>': 
                    Pop(OPTR,&optr);
                    Pop(OPND,&opnd2);
                    if(optr.flag == 2) 
                    { 
                        Pop(OPND,&opnd1);
                    } 
                    Push(OPND,Operate(opnd1,optr,opnd2));
            } 
        } 
        else 
        { 
            if(token.str[0] == '~')
            {
                strcpy(token.str,token.str+1);
                opnd1.opnd = (double)(~(int)atof(token.str));
            }
            else        
                opnd1.opnd = atof(token.str);
            Push(OPND,opnd1);
            gettoken(); 
        } 
    }
    result = OPND->next->date.opnd; 
    free(expr.str);
    DestroyStack(&OPTR);
    DestroyStack(&OPND);
    return result;
} 


/**************************************************************************************************************************************/
void InitMouse()
{
    int *x, *y;

  union REGS r;
    r.x.ax = 0;
    int86(0x33, &r, &r);

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

void MousePosition(int *x, int *y)
{
    union REGS r;
    r.x.ax = 3;
    int86(0x33, &r, &r);
    *x = r.x.cx;
    *y = r.x.dx;
}

int LeftPress()
{
    union REGS r;
    r.x.ax= 3;
    int86(0x33, &r, &r);
    
    return r.x.bx&1;
}

int RightPress()
{
    union REGS r;
    r.x.ax= 3;
    int86(0x33, &r, &r);
    delay(30000);
    return r.x.bx&2;
}

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
void InitGraph()
{
  int driver = VGA, mode = VGAHI;
  initgraph(&driver, &mode, "");
}

void welcome()
{
    int p,q;
    int pp=0,qq=0;
    int a=320, b=233;

    unsigned long gsize;

    setfillstyle(SOLID_FILL, 15);
    bar(a, b, a+MSIZEX, b+MSIZEY);
    gsize= imagesize(a, b, a+MSIZEX, b+MSIZEY);
    imagep=malloc(gsize);
    imagep1=malloc(gsize);
    getimage(a, b, a+MSIZEX, b+MSIZEY, imagep);
    setfillstyle(SOLID_FILL, 0);
    bar(a, b, a+MSIZEX, b+MSIZEY);
    
    setbkcolor(0);
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
    while(1)
    {
        MousePosition(&p,&q);
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

void UI()
{
    int i, j;
    int fontd= 0;

    setbkcolor(0);

    setfillstyle(SOLID_FILL, 0);
    bar(0, 0, 640, 640);

    setfillstyle(SOLID_FILL, 17); 
    bar(0+BLANK, 0+BLANK, 640-BLANK, 2+BLANK);

    setfillstyle(SOLID_FILL, 8); 
    bar(0+BLANK,2+BLANK, 640-BLANK, SCREENWIDTH+BLANK);

    j=0; 
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

    setfillstyle(SOLID_FILL, 2);
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

    j=0; 
    setfillstyle(SOLID_FILL, 1);
    for(i=0; i<2; i++)
    {
        bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }

    setfillstyle(SOLID_FILL, 4);
    for(i=2; i<4; i++)
    {
        bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }


    setfillstyle(SOLID_FILL, 8); 
    for(i=0; i<6; i++)
    {
        for(j=1; j<6; j++)
        {
            bar(i*(BUTTONX+GAP)+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (i+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }

    for(i=0; i<4; i++)   
    {
        for(j=1; j<6; j++)
        {
            bar((6+i)*(BUTTONX+GAP)+GAP+BLANK, SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK,
                (7+i)*(BUTTONX+GAP)+BLANK, SCREENWIDTH+(j+1)*(BUTTONY+GAP)+GAP+2+BLANK);
        }
    }

    settextstyle(2, 0, 7);
    setcolor(15);
    for(i=0; i<6; i++)
    {
        for(j=0; j<6; j++)
        {
            if(strlen(buttonleft[j*6+i])==0) continue;
            else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
            else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
            else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
            else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
            else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

            if(i==1&&j==4) 
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);
                
            else if(i==2&&j==4) 
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                
            else if(i==5&&j==4) 
                outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
               
            else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
             
        }
    }
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);

    for(i=0; i<4; i++)
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
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);

    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);


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
}


/**************************************************************************************************************************************/
int conp=1,conq=4;

void buttondown(int i, int j,int LR){
    int fontd= 0;
    int x,y,color;
    int flagi,flagj;
    int o;
    if (LR==0)
    {
        x = i*(BUTTONX+GAP)+BLANK;
        y = j*(BUTTONY+GAP)+140;
    }
    else if (LR==1)
    {
        x = i*(BUTTONX+GAP)+386;
        y = j*(BUTTONY+GAP)+140;
    }
    color = 15;
    setfillstyle(SOLID_FILL, color);
                                                                
    if((LR==0&&j!=0)||(LR==1))
    {
        if(((LR==0)&&(i==5)&&(j==3)))
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
    else if((LR==0)&&(i<=3)&&(j==0))
    {                  
        conp=i;
        bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    else if((LR==0)&&(i>3)&&(i<6)&&(j==0))
    {
        conq=i;
        bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    flagi=i;
    flagj=j;
    settextstyle(2, 0, 7);

    if(!((LR==0)&&(i==5)&&(j==3)))
    {                               
        if(shift==0)
        {
            for(i=0; i<6; i++)
            {
                for(j=0; j<6; j++)
                {   
                    if(LR==0&&i==flagi&&j==flagj) setcolor(0);         
                    if(strlen(buttonleft[j*6+i])==0) continue;
                    else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                    else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                    else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                    else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                    else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

                    if(i==1&&j==4) 
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                    else if(i==2&&j==4) 
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else if(i==5&&j==4) 
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);  
                        setcolor(15);
                }
            }
        }

        if(shift==1)
        {
            for(i=0; i<6; i++)
            {
                for(j=0; j<6; j++)
                {   
                    if(LR==0&&i==flagi&&j==flagj)setcolor(0);                           
                    if(!((j==2)&&(i<=4)))
                    {
                        if(strlen(buttonleft[j*6+i])==0) continue;
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

                    if(i==1&&j==4)
                    {
                        fontd=1;
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                    }  
                    else if(i==2&&j==4)
                    {
                        fontd=1;
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                    } 
                    else if(i==5&&j==4)
                    { 
                        fontd=2;
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);    
                    }    
                    else if(!((j==2)&&(i<=4)))
                    {
                        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                    }
                    else if((j==2)&&(i<=4))
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
                }
            }
        }
        setcolor(15);
    }
    else if(((LR==0)&&(i==5)&&(j==3)))
    {
        fontd=5;
        setcolor(0);
        outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
            SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
        setcolor(15);
    }

    if(LR==0&&flagi==1&&flagj==3) setcolor(0);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
             1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    setcolor(15);

    for(i=0; i<4; i++)
    {
        for(j=0; j<6; j++)
        {
            if(!((j==0)&&((i<=1)||(i==3))))
            {
                if(LR==1&&i==flagi&&j==flagj) setcolor(0);   
            }
            if(strlen(buttonright[j*4+i])==0) continue;
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

    if(LR==1&&flagi==0&&flagj==0) setcolor(0);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);
    setcolor(15);

    if(LR==1&&flagi==1&&flagj==0) setcolor(0);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
            (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);
    setcolor(15);

    if(LR==1&&flagi==3&&flagj==0)setcolor(0);/*删除符号*/
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


void buttonup(int i, int j,int LR){
    int x,y,color;
    int o;
    int fontd=0;
    color = 8;
    if(LR==0&&j==0&&(i>=0)&&(i<=3)) color = 9;
    else if(LR==0&&j==0&&(i>=4)&&(i<=5)) color = 2;
    else if(LR==1&&j==0&&(i>=0)&&(i<=1)) color = 1;
    else if(LR==1&&j==0&&(i>=2)&&(i<=3)) color = 4;
                                                                
    if (LR==0)
    {
        x = i*(BUTTONX+GAP)+BLANK;
        y = j*(BUTTONY+GAP)+140;
    }
    else if (LR==1)
    {
        x = i*(BUTTONX+GAP)+386;
        y = j*(BUTTONY+GAP)+140;
    }
    setfillstyle(SOLID_FILL, color);
                                                                
    if(((LR==0)&&(j!=0))||(LR==1))
    {
        if(!((LR==0)&&(i==5)&&(j==3))&&(!((LR==0)&&(j==2)&&(i<=4)))) bar(x,y,x+BUTTONX,y+BUTTONY);
    }
    else if((i<=3)&&(j==0))
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
    else if((i>3)&&(i<6)&&(j==0))
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

    if(shift==1) setfillstyle(SOLID_FILL, 3);
    else if(shift==0) setfillstyle(SOLID_FILL, 8);

    for(o=0; o<5; o++)
    {
        bar(o*(BUTTONX+GAP)+BLANK, SCREENWIDTH+2*(BUTTONY+GAP)+2*GAP+2+BLANK,
            (o+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(2+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    }
    bar(5*(BUTTONX+GAP)+BLANK, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK,
        (5+1)*(BUTTONX+GAP)+BLANK-GAP, SCREENWIDTH+(3+1)*(BUTTONY+GAP)+GAP+2+BLANK);
    settextstyle(2, 0, 7);

    if(shift==0)
    {   
        for(i=0; i<6; i++)
        {
            for(j=0; j<6; j++)
            {
                if(strlen(buttonleft[j*6+i])==0) continue;
                else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                else if(strlen(buttonleft[j*6+i])==5) fontd= 5;

                if(i==1&&j==4) 
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                else if(i==2&&j==4) 
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(i==5&&j==4) 
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                    SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
            }
        }
    }

    if(shift==1)
    {
        for(i=0; i<6; i++)
        {
            for(j=0; j<6; j++)
            {   
                if(!((j==2)&&(i<=4)))
                {
                    if(strlen(buttonleft[j*6+i])==0) continue;
                    else if(strlen(buttonleft[j*6+i])==1) fontd= 1;
                    else if(strlen(buttonleft[j*6+i])==2) fontd= 2;
                    else if(strlen(buttonleft[j*6+i])==3) fontd= 3;
                    else if(strlen(buttonleft[j*6+i])==4) fontd= 4;
                    else if(strlen(buttonleft[j*6+i])==5) fontd= 5;
                }
                else if(((j==2)&&(i<=4)))
                {
                    if(strlen(extra[i]==3)) fontd = 3;
                    else if(strlen(extra[i]==4)) fontd = 4;
                }

                if(i==1&&j==4)                                                                 
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10-2, buttonleft[j*6+i]);   
                else if(i==2&&j==4) 
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+6,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(i==5&&j==4)
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5+3,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);     
                else if(!((j==2)&&(i<=4)))
                    outtextxy(i*(BUTTONX+GAP)+BLANK+BUTTONX/2-fontd*5,
                        SCREENWIDTH+j*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-10, buttonleft[j*6+i]);
                else if((j==2)&&(i<=4))
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
            }
        }
    }

    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2-9+4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);
    line(1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12,
        1*(BUTTONX+GAP)+BLANK+BUTTONX/2+9-4, SCREENWIDTH+3*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6+12);

    for(i=0; i<4; i++)
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
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+0)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);

    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2+6);
    line((6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2+8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2,
        (6+1)*(BUTTONX+GAP)+GAP+BLANK+BUTTONX/2-8, SCREENWIDTH+0*(BUTTONY+GAP)+2*GAP+2+BLANK+BUTTONY/2-6);


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

}

void show(int i, int j, int LR)
{
    void instring(int i, int j, int LR);

    buttondown( i,  j,  LR);
    delay(1000);
    buttonup(i, j, LR);
    if(LR==0&&j==3&&i==5);
    else if(j!=0) outequation(i, j ,LR);
    else if(LR==1&&j==0&&i==2) clearscreen();
    else if(LR==1&&j==0&&i==3) backspace();
    instring(i, j, LR);
}


/**************************************************************************************************************************************/
#define CHARWIDTH 12
#define HELPW 17
#define HELPL 80

int th =0;
int th1=0;
int k=-1;

char back[150][7];


char buttonleftout[36][7]={"Bin", "Oct", "Dec", "Hex", "Rad", "Deg",
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
extern char extraout[5][7]={"asin(", "acos(", "atan(","10^","exp("};

char buttonleftstring[36][7]={"", "", "", "", "", "",
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

extern char binaryoperator[11]={'/', '*', '-', '+', '<', '>', 'X', '|', '&', 'm', '^'};


void outequation(int i, int j, int LR)
{
    
    if(th<50)
    {
        if(shift==1&&LR==0&&j==2&&i<=4)
        {
            outtextxy(20+th*CHARWIDTH, 17, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        else if(LR==0)
        {
            outtextxy(20+th*CHARWIDTH, 17, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)
        {
            outtextxy(20+th*CHARWIDTH, 17, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=50&&th<100)
    {
        if(shift==1&&LR==0&&j==2&&i<=4)
        {
            outtextxy(20+th*CHARWIDTH, 34, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        else if(LR==0)
        {
            outtextxy(20+(th%50)*CHARWIDTH, 34, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)
        {
            outtextxy(20+(th%50)*CHARWIDTH, 34, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=100)
    {
        if(shift==1&&LR==0&&j==2&&i<=4)
        {
            outtextxy(20+th*CHARWIDTH, 51, extraout[i]);
            th+=strlen(extraout[i]);
            strcpy(back[++k],extraout[i]);
        }
        if(LR==0)
        {
            outtextxy(20+(th%50)*CHARWIDTH, 51, buttonleftout[j*6+i]);
            th+=strlen(buttonleftout[j*6+i]);
            strcpy(back[++k],buttonleftout[j*6+i]);
        }
        else if(LR==1)
        {
            outtextxy(20+(th%50)*CHARWIDTH, 51, buttonrightout[j*4+i]);
            th+=strlen(buttonrightout[j*4+i]);
            strcpy(back[++k],buttonrightout[j*4+i]);
        }
    }
    if(th>=150)
    {
        outtextxy(20, 68, "The formula is too long. Click \"Clear\" and restart.");
    }
}

void backspace(void)
{
    
    if(k<=0) clearscreen();
    else
    {
        setfillstyle(SOLID_FILL,8);
        bar(20+(th%50)*CHARWIDTH,(th/50+1)*17,20+(th%50)*CHARWIDTH-strlen(back[k])*CHARWIDTH,(th/50+1)*17+20);
    }
    if(th>0&&th1>0)
    {
        th-=strlen(back[k--]);
        th1-=strlen(back[k--]);
    }
}

void clearscreen(void)
{
    setfillstyle(SOLID_FILL, 17); 
    bar(0+BLANK, 0+BLANK, 640-BLANK, 2+BLANK);

    setfillstyle(SOLID_FILL, 8); 
    bar(0+BLANK,2+BLANK, 640-BLANK, SCREENWIDTH+BLANK);
    th=0;
    th1=0;
    memset(input, '\0', sizeof(input));
    memset(output, '\0', sizeof(output));
}

void instring(int i, int j, int LR)
{
    if(LR==0)
    {
        if(shift==1&&j==2)
        {
            strcat(input, extrastring[i]);
            th1+= strlen(extrastring[i]);
            input[th1]='\0';
        }
        else if(j==3&&i==4)
        {
            strcat(input, Ans);
            th1+= strlen(Ans);
            input[th1]=='\0';
        }
        else
        {
            strcat(input, buttonleftstring[j*6+i]);
            th1+= strlen(buttonleftstring[j*6+i]);
            input[th1]='\0';
        }
    }
    if(LR==1)
    {
        strcat(input, buttonrightstring[j*4+i]);
        th1+= strlen(buttonrightstring[j*4+i]);
        input[th1]='\0';
    }
}

void outstring()
{
    int p,q;
    int pp=0,qq=0;
    void clearscreen(void);
    settextstyle(2, 0, 9);
    outtextxy(618-strlen(output)*16, 95, output);
    while(1)
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

int judge(char *input)
{
    int i=0, j=0, m=0, flag=1;
    int len =strlen(input);

    for(m=0; m<len; m++)
    {
        if(input[m]=='(') i++;
        if(input[m]==')') j++;
    }
    if(i!=j) return 0;

    for(m=0; m<11; m++)
    {
        if(input[0]==binaryoperator[m]||input[len-2]==binaryoperator[m]) return 0;
    }

    for(j=1; j<len-3; j++)
    {
        if(input[j]==')'&&input[j-1]=='(') return 0;
        else if(input[j]=='('&&input[j+1]==')') return 0;
    }
    return 1;
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
    
    while(key!=0x011b)
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
                sprintf(output, "%.4f", result1);
                strcpy(Ans, output);
                outstring();
            }
        }
        if(bioskey(1)!=0) key=bioskey(0);
  }
    closegraph();
}
