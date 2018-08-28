//验证getchar()!=EOF的值是0还是1,打印EOF的值
#include "stdio.h"

int main()
{
	int is_true = (getchar()!=EOF);
	printf("The Value of \"getchar()!=EOF\" is %d, The Value of EOF is %d\n",is_true,EOF);
	return 0;
}
