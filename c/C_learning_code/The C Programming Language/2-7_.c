//0828
//��д����invert(x,p,n)����x�������²�������x�дӵ�pλ��ʼ�ĵ�n���������ƣ�λ�����󷴣�x�������λ���ֲ���
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