//0908
//使用数组和指针分别实现strcpy()的功能

#include "stdio.h"
#define MAX 100

int strcpy_(char from[], char to[])
{
	int i;
	for(i=0; from[i]!='\0'; i++)
	{
		to[i] = from[i];
	}
	to[i] = '\0';
	return 0;
}

int str_cpy_(char *from, char *to)
{
	while(*from != '\0')
	{
		*to = *from;
		from++;
		to++;
	}
	*to = '\0';
	return 0;
}

int main()
{
	char s1[] = "Hello World";
	char s2[MAX];
	char s3[] = "hello world";
	char s4[MAX];
	strcpy_(s1,s2);
	str_cpy_(s3,s4);
	printf("s1: %s\ncopy to s2,\ns2: %s\n",s1,s2);
	printf("s3: %s\ncopy to s4,\ns4: %s\n",s3,s4);
	return 0;
}