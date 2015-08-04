#include <media/mediaplayer.h>
#include <media/IMediaPlayer.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <gui/IGraphicBufferProducer.h>
#include <gui/Surface.h>
#include <utils/String8.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

using namespace android;

int main(int argc, char **argv)
{
	sp < ProcessState > proc = ProcessState::self();
	proc->startThreadPool();
	printf("entering main...\n");
	MediaPlayer mediaplayer;
	sp <Surface> gsf;
	status_t nState;
	int fd, fsize;

	fd = open( argv[1], O_RDONLY );
	if( fd < 0 )
	{
		perror("open()");
		exit(-1);
	}
	fsize = lseek( fd, 0, SEEK_END );
	printf("fsize=%d\n", fsize );
	mediaplayer.setDataSource(fd,0,fsize);
	close(fd);

	printf("create SurfaceComposerClient\n");

	sp<SurfaceComposerClient> composerClient;
	sp<SurfaceControl> control;

	composerClient = new SurfaceComposerClient;
	composerClient->initCheck();

	printf("create video surface\n");

	control = composerClient->createSurface(
			String8("A Surface"),
			1280,
			800,
			PIXEL_FORMAT_RGBA_8888,
			0);

	SurfaceComposerClient::openGlobalTransaction();
	control->setLayer(INT_MAX);
	control->show();
	SurfaceComposerClient::closeGlobalTransaction();


	gsf = control->getSurface();
	sp<IGraphicBufferProducer> videoSurfaceTexture = gsf->getIGraphicBufferProducer();
	printf("set video surface to player\n");

	mediaplayer.setVideoSurfaceTexture(videoSurfaceTexture);

	status_t retCode = mediaplayer.prepare();

	if (retCode < 0)
	{
		printf("prepare failed: %d\n", retCode);
		IPCThreadState::self()->stopProcess(0);
		return -1;
	};

	mediaplayer.start();
	for (int i = 0; i < 4*60; i++)
	{
		sleep(1);
	}
	mediaplayer.reset();

	// close binder fd, still need waiting for all binder threads exit?
	IPCThreadState::self()->stopProcess(0);
	return 0;
}

