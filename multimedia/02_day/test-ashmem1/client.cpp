#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IPCThreadState.h>
#include <stdio.h>

using namespace android;

int main()
{
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder = sm->getService( String16("my.ashmem1") );
	sp<IMemoryHeap> heap = interface_cast<IMemoryHeap>(binder); 
	char *p = (char*)heap->getBase();
	printf("[%s]\n", p );
	return 0;
}
