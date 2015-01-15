
#ifndef WAVEDECODER_HPP_
#define WAVEDECODER_HPP_

#include <memory>
#include <vector>

#ifndef AUDIO_FORMAT_DEFINED
#define AUDIO_FORMAT_DEFINED

struct AudioFormat
{
	int channels;
	int sampleRate;
	int bitsPerSample;
};

#endif // AUDIO_FORMAT_DEFINED

bool WaveReadFile(
	const char* fileName,
	AudioFormat& outFormat,
	std::vector<char>& outData);

#endif // WAVEDECODER_HPP_

