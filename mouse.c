#include <stdio.h>
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
    r.x.ax = 0;				/*鼠标零号功能：鼠标复位*/
    int86(0x33, &r, &r);	/*调用0x33号中断*/

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
    r.x.ax = 3;				/*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);	
    *x = r.x.cx;			/*鼠标x坐标存于指针x所指的单元*/
    *y = r.x.dx;			/*鼠标y坐标存于指针y所指的单元*/
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
    r.x.ax= 3;				/*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);
    
    return r.x.bx&1;		/*单击了左键函数值为1*/
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
    r.x.ax= 3;				/*鼠标器3号功能：取按钮状态和鼠标位置*/
    int86(0x33, &r, &r);
    delay(30000);
    return r.x.bx&2;		/*单击了右键函数值为1*/
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