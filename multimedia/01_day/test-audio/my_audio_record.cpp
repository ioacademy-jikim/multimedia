#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <media/AudioRecord.h>

using namespace android;

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

int capturing = 1;

unsigned int capture_sample(FILE *file, 
                            unsigned int channels, 
							unsigned int rate,
                            unsigned int period_size,
                            unsigned int period_count);

void sigint_handler(int sig)
{
	printf("sigint_handler(%d)\n", sig );
    capturing = 0;
}

int main(int argc, char **argv)
{
    FILE *file;
    struct wav_header header;
    unsigned int channels = 2;
    unsigned int rate = 44100;
    unsigned int bits = 16;
    unsigned int frames;
    unsigned int period_size = 1024;
    unsigned int period_count = 4;

	printf("argc=%d\n", argc );
    file = fopen(argv[1], "wb");
    if (!file) {
        fprintf(stderr, "Unable to create file '%s'\n", argv[1]);
        return 1;
    }

    header.riff_id = ID_RIFF;
    header.riff_sz = 0;
    header.riff_fmt = ID_WAVE;
    header.fmt_id = ID_FMT;
    header.fmt_sz = 16;
    header.audio_format = FORMAT_PCM;
    header.num_channels = channels;
    header.sample_rate = rate;

    header.bits_per_sample = 16;
    header.byte_rate = (header.bits_per_sample / 8) * channels * rate;
    header.block_align = channels * (header.bits_per_sample / 8);
    header.data_id = ID_DATA;

    fseek(file, sizeof(struct wav_header), SEEK_SET);

    signal(SIGINT, sigint_handler);
    frames = capture_sample(file, 
			                header.num_channels,
                            header.sample_rate, 
                            period_size, 
							period_count);
    printf("Captured %d frames\n", frames);

    header.data_sz = frames * header.block_align;
    header.riff_sz = header.data_sz + sizeof(header) - 8;
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(struct wav_header), 1, file);

    fclose(file);

    return 0;
}

unsigned int capture_sample(FILE *file, 
                            unsigned int channels, 
							unsigned int rate,
                            unsigned int period_size,
                            unsigned int period_count)
{
	char *buffer;
	unsigned int size;
	unsigned int bytes_read=0;
	int kMaxBufferSize = 2048;
	size_t minFrameCount;
	status_t status = AudioRecord::getMinFrameCount(&minFrameCount,
			rate,
			AUDIO_FORMAT_PCM_16_BIT,
			audio_channel_in_mask_from_count(channels));
	if (status == OK) {
		uint32_t frameCount = kMaxBufferSize / sizeof(int16_t) / channels;

		size_t bufCount = 2;
		while ((bufCount * frameCount) < minFrameCount) {
			bufCount++;
		}

		sp<AudioRecord> record = new AudioRecord(
				AUDIO_SOURCE_DEFAULT, rate, AUDIO_FORMAT_PCM_16_BIT,
				audio_channel_in_mask_from_count(channels),
				(size_t) (bufCount * frameCount)	
				);
		status = record->initCheck();
		if(status != NO_ERROR) {
			record.clear();
			printf("Failed for initCheck()");
			return 0;
		}


		printf("Capturing sample: %u ch, %u hz, %u bit\n", channels, rate,
				16);

		size = period_count * period_size * channels * 2; 
		buffer = (char*)malloc(size); 

		record->start();

		while (capturing) {
			size = record->read( buffer, size);
			if (fwrite(buffer, 1, size, file) != size) {
				fprintf(stderr,"Error capturing sample\n");
				break;
			}
			bytes_read += size;
		}

		free(buffer);
	}
	return bytes_read/4;
}

