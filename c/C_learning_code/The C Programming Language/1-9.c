//0826
//��дһ�������븴�Ƶ�����ĳ��򣬲������������Ķ���ո���һ���ո����

#include "stdio.h"

int main()
{
	char c;
	c=getchar();
	//putchar(c);
	int trigger = 1;
	while(c!=EOF)
	{
		if(c==' ')
		{
			c = getchar();
			if(c == ' ')
			{
				if(trigger)
					putchar(c);
				trigger = 0;
				continue;
			}
			else
				putchar(c);
				trigger = 1;
		}
		else
			putchar(c);
			c = getchar();
			trigger =1;
	}
	return 0;
}