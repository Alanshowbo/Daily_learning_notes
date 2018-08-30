//0826
//编写一个将输入复制到输出的程序，并将其中连续的多个空格用一个空格代替

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