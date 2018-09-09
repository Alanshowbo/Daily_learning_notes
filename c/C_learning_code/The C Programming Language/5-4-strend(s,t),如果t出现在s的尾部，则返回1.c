//0908
//编写函数strend(s,t),如果t出现在s的尾部，则返回1，否则返回0

#include "stdio.h"

int strend(char *s, char *t)
{
	int i,j;
	for(i=0;*(s+i)!='\0';i++)
		;
	//printf("s length: %d\n", i);//i为s中的字符数
	for(j=0;*(t+j)!='\0';j++)
		;
	if(t>s)
		return 0;
	else
	{
		--i,--j;//不比较空字符
		for(;(*(s+i)==*(t+j))&&j>=0;--i,--j)
			printf("*(s+%d):%c, *(t+%d):%c",i,*(s+i),j,*(t+i));
		if(j<=0)
			return 1;
		else
			return 0;
	}
}

int main()
{
	char s1[] = "Hello World!";
	char s2[] = "World!";
	char s3[] = "worlD";
	printf("s1:%s, s2:%s, s3:%s\n",s1,s2,s3);
	printf("strend(s1,s2): %d\nstrend(s1,s3): %d\n",strend(s1,s2),strend(s1,s3));
}