//��дitob(n,s,16)������nת��Ϊ��bΪ�׵���������ת��������ַ�����ʽ���浽�ַ���s�С�
//���ܴ���16����
#include "stdio.h"
#define MAX 100

int reverse_(char s[])
{
	int i = 0;
	while(s[i]!='\0')
		i++;
	for(int t=0;t<i/2;t++)
	{
		char temp = s[t];
		s[t] = s[i-1-t];
		s[i-t-1] = temp;
	}
	return 0;
}

int itob(int n, int b, char s[])
{
	int i = 0;
	if(n<b)
	{
		s[i++] = '0'+n;
		s[i] = '\0';
	}
	else
	{
		while(n!=0)
		{
			s[i++] = '0' + n%b;
			n/=b;
		}
		s[i] = '\0';
		reverse_(s);
	}
	return 0;
}

int main()
{
	char s[MAX];
	itob(100,6,s);//244
	printf("Result: %s\n",s);
	return 0;
}