//0829
//编写strindex(char s[],char t[])函数，返回t在s中的位置，若未找到则返回-1
#include "stdio.h"
#define MAX 100

int strindex(char s[],char t[])
{
	int i,j,k;
	for(int i=0; s[i]!='\0'; i++)
	{
		for(j=i,k=0; t[k]!='\0'&&t[k]==s[j];j++,k++)
			;
		if(k>0&&t[k]=='\0')
			return i;
	}
		
	return -1;
}

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

int main()
{
	char s1[MAX];
	char s2[MAX];
	printf("enter line 1:\n");
	getline_(s1,MAX);
	printf("\nenter line 2:\n");
	getline_(s2,MAX);
	printf("\nindex result: %d\n",strindex(s1,s2));
	return 0;
}
