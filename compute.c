#include <calculator.h>

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