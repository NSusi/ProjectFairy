#ifndef GRAPHICS_H_INLCUDED
#define GRAPHICS_H_INCLUDED

#include <Windows.h>
#include <DxErr.h>
#pragma comment(lib, "DxErr.lib")

#endif

#include <Dsound.h>
#pragma comment(lib, "Dsound.lib")
#include "WaveDecoder.h"
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
const int StreamBufferSize = 32768; // 32 kb
const int StreamUpdateSize = StreamBufferSize / 2; // Update when half the buffer has played

#ifndef SOUND_H_INLCUDED
#define SOUND_H_INCLUDED

class Sound
{
private:
	HWND theHWND; //kanske är överflödigt att spara på den nu
	LPDIRECTSOUND8 ds8;
	LPDIRECTSOUNDBUFFER dsbuffer;
	int writeCursor;
	int readCursor;
	std::vector<char> dataBuffer;
public:
	Sound();
	bool ActivateSound(HWND hwnd);
	bool LoadThePCMSound(float nrOfBuffSecs = 1.0, int frequency = 440);
	bool LoadWaveSound(const char* filename, float nrOfBuffSecs = 1.0);
	bool FillWaveBuffer(char* src);
	bool OpenStream(const char* filename);
	bool CloseStream(){ this->ReleaseSoundStuff(); }
	bool UpdateStream(bool forceUpdate = false);
	bool FillSoundBuffer(char* src, int srcLength);
	bool PlayTheSound(bool isLoop);
	bool StopTheSound();
	void ReleaseSoundStuff();
	~Sound();
};

#endif