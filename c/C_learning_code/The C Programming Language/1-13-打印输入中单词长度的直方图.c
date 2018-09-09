//0826
//编写一个程序，打印输入中单词长度的直方图
#include <stdio.h>

void putstar(int ct)
{
	for(int i = 0; i<ct; i++)
	{putchar('*');}
}

int main()
{
	int count = 0;
	char c;
	printf("Enter a sentence:\n");
	while((c=getchar())!=EOF)
	{
		if((c>='A'&&c<='Z') || (c>='a'&&c<='z'))
		{
			count++;
			putchar(c);
		}
		else
		{
			printf("\t%d\t",count);
			putstar(count);
			count = 0;
			printf("\n");
		}
	}
	return 0;
}