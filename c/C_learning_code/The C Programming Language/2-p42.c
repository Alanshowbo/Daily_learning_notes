//0828
//ʹ���������ʽʵ�֣���ӡ����Ԫ�أ�ÿ�д�ӡ10����ÿ��֮��ո������ÿ��Ϊ���з�����
#include "stdio.h"

int main()
{
	int s[25];
	for(int i = 0; i<25; i++)
	{
		s[i] = i;
	}
	for(int i = 0; i<25; i++)
	{
		printf("%3d",s[i]);
		printf("%c",(i%10 == 9 || i==25-1)?'\n':' ');
	}
	return 0;
}
