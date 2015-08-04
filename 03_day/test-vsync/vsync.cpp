#include <gui/DisplayEventReceiver.h>
#include <utils/Looper.h>
#include <android/looper.h>

using namespace android;

int receiver(int fd, int events, void* data)
{
    DisplayEventReceiver* q = (DisplayEventReceiver*)data;

    ssize_t n;
    DisplayEventReceiver::Event buffer[1];

    static nsecs_t oldTimeStamp = 0;

    while ((n = q->getEvents(buffer, 1)) > 0) {
        for (int i=0 ; i<n ; i++) {
            if (buffer[i].header.type == DisplayEventReceiver::DISPLAY_EVENT_VSYNC) {
                printf("event vsync: count=%d\t", buffer[i].vsync.count);
            }
            if (oldTimeStamp) {
                float t = float(buffer[i].header.timestamp - oldTimeStamp) / s2ns(1);
                printf("%f ms (%f Hz)\n", t*1000, 1.0/t);
            }
            oldTimeStamp = buffer[i].header.timestamp;
        }
    }
    if (n<0) {
        printf("error reading events (%s)\n", strerror(-n));
    }
    return 1;
}

int main(int argc, char** argv)
{
    DisplayEventReceiver myDisplayEvent;


    sp<Looper> loop = new Looper(false);
    loop->addFd(myDisplayEvent.getFd(), 0, ALOOPER_EVENT_INPUT, receiver,
            &myDisplayEvent);

    myDisplayEvent.setVsyncRate(1);

    do {
        //printf("about to poll...\n");
        int32_t ret = loop->pollOnce(-1);
        switch (ret) {
            case ALOOPER_POLL_WAKE:
                //("ALOOPER_POLL_WAKE\n");
                break;
            case ALOOPER_POLL_CALLBACK:
                //("ALOOPER_POLL_CALLBACK\n");
                break;
            case ALOOPER_POLL_TIMEOUT:
                printf("ALOOPER_POLL_TIMEOUT\n");
                break;
            case ALOOPER_POLL_ERROR:
                printf("ALOOPER_POLL_TIMEOUT\n");
                break;
            default:
                printf("ugh? poll returned %d\n", ret);
                break;
        }
    } while (1);

    return 0;
}
