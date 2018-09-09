//0828
//编写bitcount()函数，统计x中值为1的二进制位数
//在求对二的补码时，表达式 x &= (x - 1) 可以删除 x 中最右边值为 1 的一个二进制位

#include"stdio.h"


int bitcount(unsigned x)
{
	int b;//0100101
	for (b = 0; x != 0; b++)
		x&=(x-1);
	return b;
	
	//常规实现
	/*
	int i;
	for(i=0; t!=0; t>>=1)
	{
		if(t&01)
			i++;
	}
	return i;
	*/
}


int main()
{
	int t = 0177; //001111111
	printf("Results: %d\n", bitcount(t));
	return 0;
}