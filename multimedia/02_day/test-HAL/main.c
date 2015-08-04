#include <stdio.h>
#include <dlfcn.h>

int main()
{
	int *global;
	int (*func)(char*);
	int (*add)(int,int);
	int data;
	void *handle = dlopen("libatoi.so", RTLD_LAZY);

	func = dlsym( handle, "my_atoi" );
	data = func("123");
	printf("data=%d\n", data );

	add  = dlsym( handle, "my_add" );
	data = add(1,2);
	printf("data=%d\n", data );

	*global  = dlsym( handle, "global" );
	data = add(1,2);
	printf("data=%d\n", data );
	dlclose(handle);
}


