#include "main.h"


int my_atoi(char *buff)
{
	int i, sum=0;
	for(i=0; buff[i]; i++ )
		sum = sum*10 + buff[i] - '0';

	return sum;
}


int my_add(int a, int b )
{
	return a+b;
}


struct _HMI HMI = { my_atoi, my_add, 10 };
