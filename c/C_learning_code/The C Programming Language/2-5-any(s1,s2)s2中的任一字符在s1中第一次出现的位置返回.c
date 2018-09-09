//0827
//编写函数any(s1,s2),将字符串s2中的任一字符在s1中第一次出现的位置作为结果返回，若不包含则返回-1

#include "stdio.h"
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

void printline(char s[])
{
	for(int i=0; s[i]!='\0';i++)
	{
		putchar(s[i]);
	}
	printf("\n");
}

int check(char c, char s[])
{
	for(int i =0;s[i]!='\0';i++)
	{
		if(s[i] == c)
			return i+1;
	}
	return -1;
}

int pos_print(char s1[],char s2[])
{
	for(int i=0;s2[i]!='\0';i++)
	{
		printf("%c******%d\n",s2[i],check(s2[i],s1));
	}
	return 0;
}

int main()
{
	printf("enter a line:\n");
	char s1[MAX];
	getline_(s1,MAX);
	printf("\nenter another line:\n");
	char s2[MAX];
	getline_(s2,MAX);
	printf("\nResults:\n");
	pos_print(s1,s2);
	return 0;
}