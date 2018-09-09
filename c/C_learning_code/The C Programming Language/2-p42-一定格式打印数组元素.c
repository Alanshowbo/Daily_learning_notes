//0828
//使用条件表达式实现：打印数组元素，每行打印10个，每列之间空格隔开，每行为换行符结束
#include "stdio.h"

int main()
{
	int s[25];
	for(int i = 0; i<25; i++)
	{
		s[i] = i;
	}
	for(int i = 0; i<25; i++)
	{
		printf("%3d",s[i]);
		printf("%c",(i%10 == 9 || i==25-1)?'\n':' ');
	}
	return 0;
}
