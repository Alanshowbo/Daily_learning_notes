//0827
//编写atoi()、lower(),atoi()将数字字符串转换为int，lower将字符串中的大写字母转换为小写

#include "stdio.h"
#include "stdlib.h"
#define MAX 1000

int atoi_(char s[],int count)
{
	int t=0,result=0;
	for(int i=0;i<count;i++)
	{
		if(s[i]>='0'&&s[i]<='9')
		{
			t = s[i] - '0';
			result = 10*result + t;
		}
		else
		{
			printf("Input error");
		}
	}
	return result;
}

int lower(char s[],int count)
{
	for(int i=0; i<count; i++)
	{
		if(s[i]<='Z'&&s[i]>='A')
			s[i] += 'a' - 'A';
	}
	return 0;
}
/*
int getline_(char s[], int lim)
{
	char c;
	int count = 0;
	for(int i=0; c!=EOF && i<lim; i++)
	{
		c = getchar();
		if(c == '\n')
			continue;
		s[i] = c;
		count++;
	}
	s[count] = '\0';
	return count;
}
*/

int getline_(char s[], int lim)
{
	char c;
	int i = 0;
	int count = 0;
	while(((c=getchar())!=EOF)&&i<(lim-1))
	{
		s[i] = c;
		i++;
		count++;
	}
	s[i] = '\0';
	return count;
}

int printline(char s[], int lim)
{
	for(int i=0; i<lim &&(s[i]!='\0'); i++)
	{
		putchar(s[i]);
	}
	printf("\n");
	return 0;
}

int main()
{
	char s1[MAX];
	char s2[MAX];
	printf("Enter a line only contain numbers:\n");
	int t1 = getline_(s1, MAX);
	printf("line entered:\n");
	printline(s1,MAX);
	printf("characters count: %d\n",t1);
	int num = atoi_(s1,t1);
	printf("line convert to int: %d\n",num);
	printf("Enter a line;\n");
	int t2 = getline_(s2,MAX);
	lower(s2, t2);
	printf("\nLine after lower:\n");
	printline(s2,MAX);
	return 0;
}
