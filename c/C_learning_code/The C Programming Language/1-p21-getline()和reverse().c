//0826
//编写getline()和reverse()，getline()将一行读入到s中，并返回长度；reverse()实现倒序

#include "stdio.h"
#define MAX 1000

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

int reverse(char s[], int ct)
{
	int t = ct/2;
	char c;
	for(int i=0; i<t; i++)
	{
		c = s[i];
		s[i] = s[ct-1-i];
		s[ct-1-i] = c;
	}
	return 0;
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
	char c;
	char s1[MAX];
	printf("Enter a Line:\n");
	int chars_count = getline_(s1,MAX);
	printf("The line contains %d characters, the entered line is:\n", chars_count);
	printline(s1,MAX);
	reverse(s1, chars_count);
	printf("Reversed line is:\n");
	printline(s1, MAX);
	return 0;
}