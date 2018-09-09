//0828
//按位运算符示例，& | ^ ~ << >>
//编写getbits(x,p,n),返回x中从右边数第p位开始向右n位的字段
#include "stdio.h"

int getbits(int x, int p,int n)
{
	int t = (x>>(p-n))&~(~0<<n);
	return t;
}

int main()

{
	int num1 = 0177,num2 = 0171;
	printf("num1: %o, num2: %o\n",num1,num2);
	printf("num1&num2: %o\n", num1&num2);
	printf("num1|num2: %o\n", num1|num2);
	printf("num1^num2: %o\n", num1^num2);
	printf("~num1: %o\n", ~num1);
	printf("num1<<2: %o\n", num1<<2);
	printf("num2>>2: %o\n", num2>>2);
	printf("~(~0<<3): %o\n",~(~0<<3));
	printf("%o\n",getbits(num2,5,3));
}

/*
num1: 177, num2: 171
num1&num2: 171
num1|num2: 177
num1^num2: 6
~num1: 37777777600
num1<<2: 774
num2>>2: 36
~(~0<<3): 7
6
*/
