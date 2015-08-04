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
	sp<IMemory> memory = interface_cast<IMemory>(binder); 
	ssize_t offset=0;
	size_t size=0;
    sp<IMemoryHeap> heap = memory->getMemory(&offset, &size);

	char *p = (char*)heap->getBase();
	printf("[%s]\n", p+offset );
	return 0;
}
