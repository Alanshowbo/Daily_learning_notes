//��֤getchar()!=EOF��ֵ��0����1,��ӡEOF��ֵ
#include "stdio.h"

int main()
{
	int is_true = (getchar()!=EOF);
	printf("The Value of \"getchar()!=EOF\" is %d, The Value of EOF is %d\n",is_true,EOF);
	return 0;
}
