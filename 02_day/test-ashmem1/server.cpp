#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IPCThreadState.h>

using namespace android;

int main()
{
	sp<IServiceManager> sm = defaultServiceManager();
	sp<MemoryHeapBase>  heap = new MemoryHeapBase(4096);
	sm->addService( String16("my.ashmem1") , heap );

	char *p = (char*)heap->getBase();
	sprintf(p, "Hello Client!!\n" );
	IPCThreadState::self()->joinThreadPool();
	return 0;
}
