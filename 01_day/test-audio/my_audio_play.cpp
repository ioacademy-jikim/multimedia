#include <stdio.h>
#include <media/AudioTrack.h>
#include <media/IAudioTrack.h>
#include <utils/StrongPointer.h>

using namespace android;

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

struct riff_wave_header {
	uint32_t riff_id;
	uint32_t riff_sz;
	uint32_t wave_id;
};

struct chunk_header {
	uint32_t id;
	uint32_t sz;
};

struct chunk_fmt {
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
};
// ./myapp a.wav
//         argv[1]
int main(int argc, char **argv)
{
	FILE *file;
	struct riff_wave_header riff_wave_header;
	struct chunk_header chunk_header;
	struct chunk_fmt chunk_fmt;
	char *filename;
	int more_chunks = 1;
	char buff[4096];
	int ret;

	filename = argv[1];
	file = fopen(filename, "rb");

	fread(&riff_wave_header, sizeof(riff_wave_header), 1, file);
	if ((riff_wave_header.riff_id != ID_RIFF) ||
			(riff_wave_header.wave_id != ID_WAVE)) {
		fprintf(stderr, "Error: '%s' is not a riff/wave file\n", filename);
		fclose(file);
		return 1;
	}

	do {
		fread(&chunk_header, sizeof(chunk_header), 1, file);

		switch (chunk_header.id) {
			case ID_FMT:
				fread(&chunk_fmt, sizeof(chunk_fmt), 1, file);
				/* If the format header is larger, skip the rest */
				if (chunk_header.sz > sizeof(chunk_fmt))
					fseek(file, chunk_header.sz - sizeof(chunk_fmt), SEEK_CUR);
				break;
			case ID_DATA:
				/* Stop looking for chunks */
				more_chunks = 0;
				break;
			default:
				/* Unknown chunk, skip bytes */
				fseek(file, chunk_header.sz, SEEK_CUR);
		}
	} while (more_chunks);

	sp<AudioTrack> track = new AudioTrack(AUDIO_STREAM_MUSIC,// stream type
			44100,
			AUDIO_FORMAT_PCM_16_BIT,// word length, PCM
			AUDIO_CHANNEL_OUT_STEREO,
			0);

	status_t status = track->initCheck();
	if(status != NO_ERROR) {
		track.clear();
		printf("Failed for initCheck()\n");
		return -1;
	}

	// PlaybackThread 생성 , createTrack, 공유메모리 공유 
	track->start();  // Track => ActiveTrack
	while( ret = fread( buff, 1, sizeof buff,  file ))
		track->write( buff, ret );  // 음원 전송 

	fclose(file);
	return 0;
}


