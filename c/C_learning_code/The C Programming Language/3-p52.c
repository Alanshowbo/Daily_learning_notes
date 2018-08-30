//0829
//编写itoa()，将数字转化为字符串，该函数接收3个参数，第三个参数为最小字段宽度，必要时左边填充一定数量的空格

#include "stdio.h"
#include "string.h"
#define MAX 100

int itoa_(int t,char s[],int min_len)
{
	int reverse(char s[]);
	int i;
	int sign =1;
	
	if(t<0)
	{
		sign =-1;
		t*=-1;
	}
		
	for(i=0;t>0;i++)
	{
		s[i] = '0'+t%10;
		t/=10;
	}
	
	if(sign == -1)
	{
		s[i] = '-';
		i++;
		s[i] = '\0';
	}
	else
	{
		s[++i] = '\0';
	}
	
	reverse(s);
	
	if(min_len>strlen(s))
	{
		char s1[strlen(s)+1];
		
		for(int i =0; i<=strlen(s);i++)
		{
			s1[i] = s[i];
		}
		
		int space_num=min_len-strlen(s);
		
		for(int i =0; i<space_num;i++)
			s[i] = ' ';
		
		for(int i=0; s1[i]!='\0'; i++)
		{
			s[space_num+i]=s1[i];
		}
		
		s[min_len]='\0';
	}
	return 0;
}

int reverse(char s[])
{
	int len = strlen(s)-1;
	int t = strlen(s)/2;
	
	for(int i = 0;i<t;i++)
	{
		char c = s[i];
		s[i] = s[len-i];
		s[len-i] = c;
	}
	
	return 0;
}

int main()
{
	int num = -11571;
	char s[MAX];
	itoa_(num, s, 16);
	printf("Result: %s, the length of s: %d\n", s, (int)strlen(s));
}
