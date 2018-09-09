//重新编写atoi()，要求能跳过字符数组前面的空格、并处理负数
//使用标准库中的strtol()函数
#include "stdio.h"
#include "string.h"
#define MAX 100

int getline_(char s[], int lim)
{
	char c;
	int i = 0;
	while((c=getchar())!=EOF && i<lim)
	{
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	return i;
}

int atoi_(char s[])
{
	int i,sign=1,result=0;
	for(i=0;i<strlen(s)&&(s[i]==' ');i++)
		;
	if(s[i]=='-')
	{
		sign = -1;
		i++;
	}
		
	for(;i<strlen(s);i++)
	{
		result = result*10 + (s[i] - '0');
	}
	return sign*result;
}

int main()
{
	char s[MAX];
	printf("enter a line only contain spaces(before number) and num:\n");
	getline_(s,MAX);
	printf("\nResult:%d\n",atoi_(s));
	return 0;
}
