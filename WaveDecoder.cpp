
#include "WaveDecoder.h"

#include <cmath>
#include <fstream>
#include <vector>


namespace // anonymous
{
	int readInt32(char* buf)
	{
		return *reinterpret_cast<int*>(buf);
	}
	int readUInt32(char* buf)
	{
		return *reinterpret_cast<unsigned int*>(buf);
	}
	short readInt16(char* buf)
	{
		return *reinterpret_cast<short*>(buf);
	}
	unsigned short readUInt16(char* buf)
	{
		return *reinterpret_cast<unsigned short*>(buf);
	}

	int findChunk(std::istream& file, const char* chunkID, int end)
	{
		char buffer[8];

		while(file.tellg() < end)
		{
			file.read(buffer, 8);
			if(file.gcount() != 8)
			{
				return -1;
			}

			if(buffer[0] == chunkID[0] && buffer[1] == chunkID[1] &&
				buffer[2] == chunkID[2] && buffer[3] == chunkID[3])
			{
				return readInt32(buffer+4);
			}

			file.seekg(-7, std::ios::cur);
		}
		return -1;
	}
		
	int readChunk(std::istream& file, char* buffer, int chunkLength, int end)
	{
		file.read(buffer, chunkLength);
		if(file.gcount() != chunkLength)
		{
			return -1;
		}
		
		if((chunkLength % 2) != 0)
		{
			char c;
			file.read(&c, 1);
		}

		return chunkLength;
	}
} // namespace anonymous


bool WaveReadFile(
	const char* fileName,
	AudioFormat& outFormat,
	std::vector<char>& outData)
{
	using namespace std;

	ifstream file(fileName, ios::in | ios::binary);
	if(! file)
	{
		return 0;
	}

	char	header[256];

	file.read(header, 12);
	if(file.gcount() != 12)
	{
		return false;
	}

	// check for RIFF
	if(header[0] != 'R' || header[1] != 'I' ||
		header[2] != 'F' || header[3] != 'F')
	{
		// not a RIFF-file
		return false;
	}

	// check for WAVE
	if(header[8] != 'W' || header[9] != 'A' ||
		header[10] != 'V' || header[11] != 'E')
	{
		// not a WAVE-file
		return false;
	}

	int end = readInt32(header+4) + (static_cast<int>(file.tellg()) - 4);
	
	int chunkLength;
	// search for "fmt " chunk
	if((chunkLength = findChunk(file, "fmt ", end)) < 0)
	{
		// error finding chunk 'fmt ', invalid wavefile
		return false;
	}

	// format chunk
	if(chunkLength > sizeof(header) || chunkLength < 16)
	{
		// Invalid WaveFormat header
		return false;
	}

	if(readChunk(file, header, chunkLength, end) != chunkLength)
	{
		return false; // unexpected end of file
	}

	short formatTag = readInt16(header);
	// check for pcm-data
	if(formatTag != 1)
	{
		// not pcm
		return false;
	}

	unsigned short channels = readUInt16(header+2);
	int samplesPerSec = readInt32(header+4);
	int avgBytesPerSec = readInt32(header+8);
	unsigned short blockAlign = readUInt16(header+12);
	unsigned short bitsPerSample = readUInt16(header+14);

	AudioFormat format;
	if(channels != 1 && channels != 2)
	{
		return false; // unsupported number of channels
	}
	format.channels = channels;

	if(bitsPerSample != 8 && bitsPerSample != 16)
	{
		return false; // unsupported number of bits per sample
	}
	format.bitsPerSample = bitsPerSample;

	format.sampleRate = samplesPerSec;

	// find data chunk
	if((chunkLength = findChunk(file, "data", end)) < 0)
	{
		// missing data chunk
		return false;
	}
	
	std::vector<char> data(chunkLength);
	file.read(&data[0], chunkLength);
	if(file.gcount() != chunkLength)
	{
		return false;
	}

	outData.swap(data);
	outFormat = format;

	return true;
}

