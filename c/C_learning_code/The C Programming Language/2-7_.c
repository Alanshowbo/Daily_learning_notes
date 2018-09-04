//0828
//编写函数invert(x,p,n)，对x进行如下操作：将x中从第p位开始的第n个（二进制）位进行求反，x的其余各位保持不变
#include "stdio.h"

int invert(int x, int p, int n)
{
	int t1 = x&(~(~0<<(p-n)));
	//printf("t1:%o\n",t1);
	int t2 = ~(x>>(p-n))&01;
	//printf("t2:%o\n",t2);
	x>>=(p-n+1);
	//printf("x:%o\n",x);
	x<<=1;
	//printf("x:%o\n",x);
	x+=t2;
	//printf("x:%o\n",x);
	x<<=(p-n);
	//printf("x:%o\n",x);
	x+=t1;
	//printf("x:%o\n",x);
	return x;
}

int main()
{
	int x = 0121;//01010001
	printf("Result: %o\n", invert(x,5,2));//01011001,131
	return 0;
}