//0827
//编写squeeze(s1,s2)，将s1中与s2中字符匹配的删除
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
			return 1;
	}
	return 0;
}

int squeeze(char s1[],char s2[])
{
	int i,j;
	for(i=j=0; s1[i]!='\0'; i++)
	{
		if(!check(s1[i],s2))
			s1[j++] = s1[i];
		s1[j] = '\0';
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
	squeeze(s1,s2);
	printf("\nSqueezed line:\n");
	printline(s1);
	return 0;
}