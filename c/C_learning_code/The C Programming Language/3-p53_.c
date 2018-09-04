//0829
//编写trim()函数，实现删除字符串尾部的空格、换行、制表符

#include "stdio.h"
#include "string.h"
#define MAX 100

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

int trim(char s[])
{
	int len;
	for(len = strlen(s)-1; len>0 ;len--)
		if(s[len]!='\n'&&s[len]!=' '&&s[len]!='\t')
		break;
	s[len+1]='\0';
	return 0;
}

int main()
{
	char s[MAX];
	getline_(s,MAX);
	trim(s);
	printf("Trimmed result:%s\n", s);
	return 0;
}