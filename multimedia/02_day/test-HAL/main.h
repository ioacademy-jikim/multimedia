
int my_atoi(char *buff);
int my_add(int a, int b);

struct _HMI
{
	int (*atoi)(char *buff);
	int (*add)(int , int );
	int global;
};
