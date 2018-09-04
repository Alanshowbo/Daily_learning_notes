//0826
//��дhtoi������ʮ������������ɵ��ַ���ת��Ϊ��Ӧ������ֵ���ַ���������������ְ���0-9, a-f, A-F
//ת��Ϊʮ��������

#include "stdio.h"
#include<ctype.h>
#define MAX 100

int htoi(char s[])
{
	int t = 0;
	int result = 0;
	for(int i = 0; s[i]!='\0'; i++)
	{
		if(isdigit(s[i]))
		{
			t = s[i] - '0';
		}
		else if(s[i]<='f'&&s[i]>='a')
		{
			t = 10 + (s[i] - 'a');
		}
		else if(s[i]<='F'&&s[i]>='A')
		{
			t = 10 + (s[i] - 'A');
		}
		else
		{
			printf("input error.\n");
			return -1;
		}
		result = 16*result + t;
	}
	return result;
}

int main()
{
	char s[MAX] = "1916Ac";//1644204
	printf("Hex: %s to Dec: %d\n", s, htoi(s));
	return 0;
}