//0826
//统计制表符、空格、换行符个数的程序
#include "stdio.h"

int main()
{
	char c;
	int ntabs=0, nspace=0, nreturn=0;
	c = getchar();
	while(c!=EOF)
	{
		switch(c)
		{
			case '\t': 
				ntabs++;
				break;
			case ' ':
				nspace++;
				break;
			case '\n':
				nreturn++;
				break;
		}
		c = getchar();
	}
	printf("The nums of tabs, spaces, returns are %d, %d, %d.\n", ntabs, nspace, nreturn);
	return 0;
}
