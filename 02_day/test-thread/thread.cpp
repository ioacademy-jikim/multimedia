#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>
#include <utils/Thread.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IPCThreadState.h>

using namespace android;


class MyThread : public Thread 
{
	public : 
		bool threadLoop()
		{
			printf("MyThread::threadLoop()\n"); 
			sleep(1);
			return true;
		}
};

int main()
{
	sp<Thread> thread = new MyThread;
	thread->run();
	getchar();
	return 0;
}
