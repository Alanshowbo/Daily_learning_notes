//0829
//实现折半查找法
//折半查找要求：存储结构一定是顺序存储 
#include "stdio.h"
#include "stdlib.h"

int binsearch(int s[], int key, int len)
{
	int left = 0;
	int right = len-1;
	/*
	int t = sizeof(s);
	printf("%d\n",t); //8
	*/
	while(left<=right)
	{
		int mid = (left + right)/2;
		if(s[mid]==key)
		{
			printf("found %d in array, the index is: %d\n", key, mid);
			return mid;
		}
		else
		{
			if(s[mid]<key)
				left = mid+1;
			else
				right = mid-1;
		}
		
	}
	printf("can not find.\n");
	return -1;
}

int main()
{
	int s[] = {2,5,8,10,13,19,23,26,28,31,34,36,38,40,41,45,48};
	int len = sizeof(s)/sizeof(s[0]);
	int key = 19;
	binsearch(s, key, len);
	return 0;
}
