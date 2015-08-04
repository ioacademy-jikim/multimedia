#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <ui/PixelFormat.h>
#include <media/mediaplayer.h>
#include <media/IMediaPlayer.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <utils/String8.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <gui/Surface.h>

using namespace android;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct BITMAPFILEHEADER {
	u16 bfType;
	u32 bfSize;
	u16 bfReserved1;
	u16 bfReserved2;
	u32 bfOffBits;
} __attribute((packed));

struct BITMAPINFOHEADER {
	u32 biSize;
	u32 biWidth;
	u32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	u32 biXPelsPerMeter;
	u32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;
} __attribute((packed));


void fillRGBA8BMP(uint8_t* dst, uint8_t* src, int w, int h, int stride, int bpp) 
{
	const size_t PIXEL_SIZE = 4;
	src = src+w*h*bpp;
	for (int y = 0; y < h; y++) {
		src -= w*bpp;
		for (int x = 0; x < w; x++) {
			off_t offset = (y * stride + x) * PIXEL_SIZE;
			dst[offset + 0] = src[x*bpp+2];
			dst[offset + 1] = src[x*bpp+1];
			dst[offset + 2] = src[x*bpp+0];
			dst[offset + 3] = 255;
		}
	}
}

int main()
{
	int fd;
	int w,h;
	BITMAPFILEHEADER  file_header;
	BITMAPINFOHEADER  DIB; 

	fd = open("bbb.bmp", O_RDONLY);
	printf("fd=%d\n", fd );
	read( fd, &file_header, sizeof file_header );
	read( fd, &DIB, sizeof DIB );
	w = DIB.biWidth;
	h = DIB.biHeight;
	printf("size=%u\n", file_header.bfSize );
	printf("BitCount=%u\n", DIB.biBitCount );
	int bpp = DIB.biBitCount/8;
	uint8_t *src = (uint8_t*)malloc( DIB.biSizeImage*bpp );
	read( fd, src,  DIB.biSizeImage*bpp );
	sp <Surface> gsf;
	status_t nState;

	printf("create SurfaceComposerClient\n");

	sp<SurfaceComposerClient> composerClient;
	sp<SurfaceControl> control;

	composerClient = new SurfaceComposerClient;
	composerClient->initCheck();

	printf("create video surface\n");

	control = composerClient->createSurface(
			String8("A Surface"),
			w,
			h,
			PIXEL_FORMAT_RGBA_8888,
			0);

	SurfaceComposerClient::openGlobalTransaction();
	control->setLayer(INT_MAX);
	control->show();
	SurfaceComposerClient::closeGlobalTransaction();


	sp<ANativeWindow> window;
	window = control->getSurface();

	ANativeWindowBuffer *anb;
	native_window_dequeue_buffer_and_wait(window.get(), &anb);

	sp<GraphicBuffer> buf(new GraphicBuffer(anb, false));

	uint8_t* img = NULL;
	buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));
	printf("width=%d, height=%d, stride=%d\n", 
			buf->getWidth(), buf->getHeight(), buf->getStride());

	fillRGBA8BMP(img, src, w, h, buf->getStride(), bpp);
	buf->unlock();
	window->queueBuffer(window.get(), buf->getNativeBuffer(), -1);
	getchar();

	free(src);
	close(fd);
}
