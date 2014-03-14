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
extern void far *imagep;
extern void far *imagep1;
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

void InitStack(Stack *S);
void DestroyStack(Stack *S);
void Push(Stack S, StackItem e);
void Pop(Stack S,StackItem *e);
void get_expr(char *s);
int IsOpnd(char ch);
void gettoken(void);
char Precede(StackItem *optr1, StackItem *optr2);
long fact(long n);
StackItem Operate(StackItem opnd1, StackItem optr, StackItem opnd2);
double compute(char *s);

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