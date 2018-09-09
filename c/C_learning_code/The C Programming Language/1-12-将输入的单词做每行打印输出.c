//0826
//编写一个程序，以每行一个单词的形式打印其输入

#include "stdio.h"

int main()
{
	printf("Enter a sentence:\n");
	char c;
	while((c = getchar())!=EOF)
	{
		if(c == ' ')
			printf("\n");
		else
			putchar(c);
	}
	return 0;
}