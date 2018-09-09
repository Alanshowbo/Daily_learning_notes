//0908
//使用指针的方式实现strcat(s,t),将t指向的字符串复制到s指向的字符串的尾部

#include"stdio.h"

int strcat_(char *s,char *t)
{
	int i,j;
	for(i=0;*(s+i)!='\0';i++)
		;
	for(j=0;*(t+j)!='\0';j++)
	{
		*(s+i+j) = *(t+j);
	}
	*(s+i+j) = '\0';
	return 0;
}

int main()
{
	char s1[] = "hello";
	char s2[] = " world";
	printf("s1:%s  s2:%s\n",s1,s2);
	strcat_(s1,s2);
	printf("after strcat() func, s1:%s\n",s1);
}