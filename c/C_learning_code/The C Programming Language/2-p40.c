//0828
//��дbitcount()������ͳ��x��ֵΪ1�Ķ�����λ��

#include"stdio.h"

int bitcount(int t)
{
	int i;
	for(i=0; t!=0; t>>=1)
	{
		if(t&01)
			i++;
	}
	return i;
}

int main()
{
	int t = 0177; //001111111
	printf("Results: %d\n", bitcount(t));
	return 0;
}