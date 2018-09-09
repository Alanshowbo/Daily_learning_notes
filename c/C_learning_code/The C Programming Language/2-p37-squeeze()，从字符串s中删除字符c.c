//0827
//编写squeeze()，从字符串s中删除字符c

#include "stdio.h"
#define MAX 1000

int squeeze(char s[],char c)
{
	int i ,j;
	for(i=j=0; s[i]!='\0'; i++)
	{
		if(s[i]!=c)
		{s[j++]=s[i];}
	}
	s[j] = '\0';
	return 0;
}

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

int main()
{
	char s1[MAX];
	printf("enter a line:\n");
	getline_(s1, MAX);
	printf("\nenter a character you want to remove from the line:\n");
	char c = getchar();
	squeeze(s1,c);
	printf("\nSqueezed line:\n");
	printline(s1);
}