//0826
//��дһ��������ÿ��һ�����ʵ���ʽ��ӡ������

#include "stdio.h"

int main()
{
	printf("Enter a sentence:\n");
	char c;
	while((c = getchar())!=EOF)
	{
		if(c == ' ')
			printf("\n");
		else
			putchar(c);
	}
	return 0;
}