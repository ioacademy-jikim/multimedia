#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <binder/IPCThreadState.h>

using namespace android;

int main()
{
	sp<IServiceManager> sm = defaultServiceManager();
	sp<MemoryHeapBase>  heap = new MemoryHeapBase(4096);
	sm->addService( String16("my.ashmem1") , new MemoryBase(heap, 100, 100) );

	char *p = (char*)heap->getBase();
	sprintf(p+100, "Hello Client!!\n" );
	IPCThreadState::self()->joinThreadPool();
	return 0;
}
