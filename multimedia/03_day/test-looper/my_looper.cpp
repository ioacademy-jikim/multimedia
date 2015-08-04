#if 1
#include <stdio.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

using namespace android;

class MyThread : public Thread {
	    sp<Looper> mLooper;
	    sp<MessageHandler> mHandler;
	public : 
		MyThread( sp<Looper> &looper , sp<MessageHandler> &handler ) 
			: mLooper(looper),mHandler(handler){}
		bool threadLoop() {
			mLooper->sendMessageDelayed( s2ns(3),  mHandler, Message(3) );
			return false;
		}
};

class MyMessageHandler : public MessageHandler
{
public:
	virtual void handleMessage(const Message& message)
	{
		printf("MyMessageHandler::handleMessage(%d)\n", message.what );
	}
};

int main()
{
	int ret;
	sp<Looper> looper = new Looper(true);
	sp<MessageHandler> handler = new MyMessageHandler;
	sp<Thread> thread = new MyThread( looper, handler );

	thread->run();

	while( 1)
	{
		ret = looper->pollOnce(-1);

		if( ret == Looper::POLL_WAKE ) 
			printf("Looper::POLL_WAKE\n");
		if( ret == Looper::POLL_CALLBACK ) 
			printf("Looper::POLL_CALLBACK\n");
	}
	return 0;
}
#endif
#if 0
#include <stdio.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

using namespace android;

class MyThread : public Thread {
	    sp<Looper> mLooper;
		int mFd;
	public : 
		MyThread( sp<Looper> &looper , int fd ) : mLooper(looper),mFd(fd){}
		bool threadLoop() {
			sleep(3);
			write( mFd, "W", 1 );
			return false;
		}
};

class CallbackHandler
{
public:
	void setCallback( sp<Looper> &looper, int fd, int events )
	{
		looper->addFd( fd, 0, events, callbackFunc, this );
	}
	static int callbackFunc(int fd, int events, void* data)
	{
		return ((CallbackHandler*)data)->handler( fd, events, data );
	}
	virtual ~CallbackHandler(){}
	virtual int handler(int fd, int events, void* data) = 0;
};

class MyCallback : public CallbackHandler
{
public:
	int handler(int fd, int events, void* data)
	{
		printf("MyCallback::handler(%d, %d, %p)\n", fd, events, data);
		return 0;
	}
};

int main()
{
	int ret;
	int fds[2];
	pipe(fds);
	MyCallback callback;
	sp<Looper> looper = new Looper(true);
	sp<Thread> thread = new MyThread( looper, fds[1] );

	callback.setCallback( looper, fds[0], Looper::EVENT_INPUT );
	thread->run();

	while( 1)
	{
		ret = looper->pollOnce(-1);

		if( ret == Looper::POLL_WAKE ) 
			printf("Looper::POLL_WAKE\n");
		if( ret == Looper::POLL_CALLBACK ) 
			printf("Looper::POLL_CALLBACK\n");
	}
	return 0;
}
#endif
#if 0
#include <stdio.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

using namespace android;

class MyThread : public Thread {
	    sp<Looper> mLooper;
		int mFd;
	public : 
		MyThread( sp<Looper> &looper , int fd ) : mLooper(looper),mFd(fd){}
		bool threadLoop() {
			sleep(3);
			write( mFd, "W", 1 );
			return false;
		}
};

class MyCallback
{
public:
	void setCallback( sp<Looper> &looper, int fd, int events )
	{
		looper->addFd( fd, 0, events, callbackFunc, this );
	}
	static int callbackFunc(int fd, int events, void* data)
	{
		return ((MyCallback*)data)->handler( fd, events, data );
	}

	int handler(int fd, int events, void* data)
	{
		printf("MyCallback::handler(%d, %d, %p)\n", fd, events, data);
		return 0;
	}
};

int main()
{
	int ret;
	int fds[2];
	pipe(fds);
	MyCallback callback;
	sp<Looper> looper = new Looper(true);
	sp<Thread> thread = new MyThread( looper, fds[1] );

	callback.setCallback( looper, fds[0], Looper::EVENT_INPUT );
	thread->run();

	while( 1)
	{
		ret = looper->pollOnce(-1);

		if( ret == Looper::POLL_WAKE ) 
			printf("Looper::POLL_WAKE\n");
		if( ret == Looper::POLL_CALLBACK ) 
			printf("Looper::POLL_CALLBACK\n");
	}
	return 0;
}
#endif
#if 0
#include <stdio.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

using namespace android;

class MyThread : public Thread {
	    sp<Looper> mLooper;
		int mFd;
	public : 
		MyThread( sp<Looper> &looper , int fd ) : mLooper(looper),mFd(fd){}
		bool threadLoop() {
			sleep(3);
			write( mFd, "W", 1 );
			return false;
		}
};


int foo(int fd, int events, void* data)
{
	printf("foo(%d, %d, %p)\n", fd, events, data);
	return 0;
}

int main()
{
	int ret;
	int fds[2];
	pipe(fds);
	sp<Looper> looper = new Looper(true);
	sp<Thread> thread = new MyThread( looper, fds[1] );

	looper->addFd( fds[0], Looper::POLL_CALLBACK, Looper::EVENT_INPUT, foo, 0 );
	thread->run();

	while( 1)
	{
		ret = looper->pollOnce(-1);

		if( ret == Looper::POLL_WAKE ) 
			printf("Looper::POLL_WAKE\n");
		if( ret == Looper::POLL_CALLBACK ) 
			printf("Looper::POLL_CALLBACK\n");
	}
	return 0;
}
#endif
#if 0
#include <stdio.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

using namespace android;

class MyThread : public Thread {
	    sp<Looper> mLooper;
	public : 
		MyThread( sp<Looper> &looper ) : mLooper(looper){}
		bool threadLoop() {
			sleep(3);
			mLooper->wake();
			return false;
		}
};
int main()
{
	int ret;
	sp<Looper> looper = new Looper(true);
	sp<Thread> thread = new MyThread( looper );
	thread->run();

	while( 1)
	{
		ret = looper->pollOnce(-1);

		if( ret == Looper::POLL_WAKE ) 
			printf("Looper::POLL_WAKE\n");
	}
	return 0;
}
#endif

