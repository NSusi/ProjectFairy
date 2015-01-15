#include "Sound.h"

Sound::Sound(/*HWND hwnd*/)
{
	ds8 = NULL;
	dsbuffer = NULL;
	writeCursor = 0;
	readCursor = 0;
}

Sound::~Sound()
{
	this->ReleaseSoundStuff();
}

bool Sound::ActivateSound(HWND hwnd)
{
	theHWND = hwnd;
	HRESULT result = DirectSoundCreate8(
		NULL, // guid för enheten, NULL för default
		&ds8, // pekare till ett IDirectSound8-interface
		NULL); // NULL

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH SOUNDACTIVATION");
		return false;
	}

	result = ds8->SetCooperativeLevel(theHWND, DSSCL_NORMAL);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH SOUNDCOOPERATION");
		return false;
	}

	return true;
}

bool Sound::LoadThePCMSound(float nrOfBuffSecs, int frequency) //Ska egentligen vara alternativ för hur ljudet skall låta och vart i buffern man ska spara ljudet. Men jag ändrade mig till att streama alla ljud mitt under utvecklingen av spelet.
{
	if(dsbuffer)
	{
		dsbuffer->Release();
		dsbuffer = NULL;
	}

	WAVEFORMATEX wf = {0}; // beskriver ljudformatet för buffern
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2; // 1 = mono, 2 = stereo
	wf.nSamplesPerSec = 44100; // samplingsfrekvens
	wf.wBitsPerSample = 16;
	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0;

	DSBUFFERDESC dsbuffdesc = {0};
	dsbuffdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbuffdesc.dwFlags = 0;
	dsbuffdesc.dwBufferBytes = wf.nAvgBytesPerSec * nrOfBuffSecs; // buffer som rymmer x antal sekunder
	dsbuffdesc.lpwfxFormat = &wf;

	HRESULT result = ds8->CreateSoundBuffer(
		&dsbuffdesc,
		&dsbuffer,
		0);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH CREATION OF SOUNDBUFFER");
		return false;
	}
	const int numSamples = 44100;
	//const int frequency = 440; // 440hz är frekvensen för A
	const float step = 1.0f / numSamples;
	float time = 0;
	short pcm[numSamples];

	for(int i = 0; i < numSamples; ++i)
	{
		float x = sin(2 * M_PI * frequency * time);
		pcm[i] = int(32768 * ((x + 1) / 2));
		time += step;
	}

	short* data;
	
	result = dsbuffer->Lock(
		0, // lås från början av buffern
		0, // lås hela buffern (DSBLOCK_ENTIREBUFFER)
		(LPVOID*)&data,
		&dsbuffdesc.dwSize,
		0, 0, // den andra pekaren används inte när vi låser hela buffern
		DSBLOCK_ENTIREBUFFER);

	std::copy(pcm, pcm + numSamples, data);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH BUFFERLOCKING");
		return false;
	}

	dsbuffer->SetVolume(500); // atten är ett värde mellan 0 och 10000 som talar om hur mycket volymen ska sänkas, där 0 är oförändrad volym och 10000 är helt tyst.
	dsbuffer->SetFrequency(44100); // frekvensen i antal hz
	dsbuffer->SetPan(0);

	result = dsbuffer->Unlock(
		0,
		0,
		0, 0);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH UNLOCKING");
		return false;
	}

	return true;
}

bool Sound::LoadWaveSound(const char* filename, float nrOfBuffSecs)
{
	// Read a complete wave file. Data could instead be continually streamed
	// from disk directly to the wave buffer
	AudioFormat format;
	if(!WaveReadFile(filename, format, dataBuffer))
	{
		return false;
	}

	WAVEFORMATEX wfx = {0};
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = format.channels;
	wfx.wBitsPerSample = format.bitsPerSample;
	wfx.nBlockAlign = format.channels * format.bitsPerSample / 8;
	wfx.nSamplesPerSec = format.sampleRate;
	wfx.nAvgBytesPerSec = format.sampleRate * wfx.nBlockAlign;

	DSBUFFERDESC dsbuffdesc = {0};
	dsbuffdesc.dwSize = sizeof(dsbuffdesc);
	dsbuffdesc.dwFlags = 0;
	dsbuffdesc.dwBufferBytes = wfx.nAvgBytesPerSec * nrOfBuffSecs; //StreamBufferSize;
	dsbuffdesc.lpwfxFormat = &wfx;
	dsbuffdesc.guid3DAlgorithm = GUID_NULL;

	//Create sound buffer compatible with above wave file
	HRESULT result = ds8->CreateSoundBuffer(
		&dsbuffdesc,
		&dsbuffer,
		0);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH BUFFER CREATION");
		return false;
	}
	
	writeCursor = readCursor = 0;
	//int numBytes = (int)dataBuffer.size();
	//int numBytes = (int)wfx.nAvgBytesPerSec * nrOfBuffSecs;
	//int numBytes = 176400 * nrOfBuffSecs;
	int numBytes = (int)dsbuffdesc.dwBufferBytes;

	// copy data from the stream to the sound buffer
	FillSoundBuffer(&dataBuffer[0], numBytes);
	//if(!FillWaveBuffer(&dataBuffer[0]))
	//	return false;
	// force an update so the buffer contains some data to be played
	//if(!UpdateStream(true))
	//{
	//	return false;
	//}

	// play the buffer with looping since we are continually streaming data
	//HRESULT hr = dsbuffer->Play(0, 0, DSBPLAY_LOOPING);

	//if(FAILED(hr))
	//{
	//	MessageBox(
	//		theHWND,
	//		DXGetErrorDescription(result),
	//		DXGetErrorString(result),
	//		MB_OK | MB_ICONERROR);
	//	OutputDebugString("ERROR WITH STREAMING SOUND");
	//	return false;
	//}

	return true;
}

bool Sound::FillWaveBuffer(char* src)
{
	unsigned char* dst1 = 0;
	DWORD dstLength1 = 0;

	HRESULT result = dsbuffer->Lock(
		0, 0,
		(LPVOID*)&dst1,
		&dstLength1,
		0, 0,
		DSBLOCK_ENTIREBUFFER);

	if(FAILED(result))
	{
		return false;
	}

	int j = 0;
	for(DWORD i = 0; i < dstLength1; ++i, ++j)
	{
		dst1[i] = src[j];
	}

	dsbuffer->Unlock(dst1, dstLength1, 0, 0);

	return true;
}


bool Sound::OpenStream(const char* filename)
{
	// Read a complete wave file. Data could instead be continually streamed
	// from disk directly to the wave buffer
	AudioFormat format;
	if(!WaveReadFile(filename, format, dataBuffer))
	{
		return false;
	}

	WAVEFORMATEX wfx = {0};
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = format.channels;
	wfx.wBitsPerSample = format.bitsPerSample;
	wfx.nBlockAlign = format.channels * format.bitsPerSample / 8;
	wfx.nSamplesPerSec = format.sampleRate;
	wfx.nAvgBytesPerSec = format.sampleRate * wfx.nBlockAlign;

	DSBUFFERDESC dsbuffdesc = {0};
	dsbuffdesc.dwSize = sizeof(dsbuffdesc);
	dsbuffdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
	dsbuffdesc.dwBufferBytes = StreamBufferSize;
	dsbuffdesc.lpwfxFormat = &wfx;
	dsbuffdesc.guid3DAlgorithm = GUID_NULL;

	// Create sound buffer compatible with above wave file
	HRESULT result = ds8->CreateSoundBuffer(
		&dsbuffdesc,
		&dsbuffer,
		0);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH BUFFER CREATION");
		return false;
	}
	
	writeCursor = readCursor = 0;

	// force an update so the buffer contains some data to be played
	if(!UpdateStream(true))
	{
		return false;
	}

	// play the buffer with looping since we are continually streaming data
	//HRESULT hr = dsbuffer->Play(0, 0, DSBPLAY_LOOPING);

	//if(FAILED(hr))
	//{
	//	MessageBox(
	//		theHWND,
	//		DXGetErrorDescription(result),
	//		DXGetErrorString(result),
	//		MB_OK | MB_ICONERROR);
	//	OutputDebugString("ERROR WITH STREAMING SOUND");
	//	return false;
	//}

	return true;
}

bool Sound::UpdateStream(bool forceUpdate)
{
	DWORD playCursor;
	HRESULT hr = dsbuffer->GetCurrentPosition(&playCursor, 0);
	if(FAILED(hr) || playCursor < 0)
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(hr),
			DXGetErrorString(hr),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH GETTING CURRENT POSITION PLAYCURSOR");
		return false;
	}

	// if play cursor is behind us, we move it (virtually) in front of us to
	// more easily calculate the distance.
	if((int)playCursor < writeCursor)
	{
		playCursor += StreamBufferSize;
	}

	// if the playcursor is more than half a buffersize in front of us, fill
	// the buffer
	if((playCursor - writeCursor) >= StreamUpdateSize || forceUpdate)
	{
		// how many bytes should be streamed to the buffer?
		int numBytes = min(StreamUpdateSize, (int)dataBuffer.size() - readCursor);
		if(numBytes == 0)
		{
			// should zero remainder of the buffer to avoid re-playing ant data
			dsbuffer->Stop();
			return false;
		}

		// copy data from the stream to the sound buffer
		FillSoundBuffer(&dataBuffer[readCursor], numBytes);

		// calculate the new readCursor position
		readCursor += numBytes;

		// calculate the new writeCursor position
		writeCursor += numBytes;
		// "wrap" the write cursor around to the beginning of the buffer
		writeCursor %= StreamBufferSize;
	}

	return true;
}

bool Sound::FillSoundBuffer(char* src, int srcLength)
{
	unsigned char* dst1 = 0;
	unsigned char* dst2 = 0;
	DWORD dstLength1 = 0;
	DWORD dstLength2 = 0;

	HRESULT result = dsbuffer->Lock(
		writeCursor,
		srcLength,
		(LPVOID*)&dst1,
		&dstLength1,
		(LPVOID*)&dst2,
		&dstLength2,
		0);
	if(FAILED(result))
	{
		return false;
	}

	int j = 0;
	for(DWORD i = 0; i < dstLength1; ++i, ++j)
	{
		dst1[i] = src[j];
	}
	for(DWORD i = 0; i < dstLength2; ++i, ++j)
	{
		dst2[i] = src[j];
	}

	dsbuffer->Unlock(dst1, dstLength1, dst2, dstLength2);

	return true;
}

bool Sound::PlayTheSound(bool isLoop)
{
	HRESULT result;
	if(isLoop)
	{
		result = dsbuffer->Play(0, 0, DSBPLAY_LOOPING);
		if(FAILED(result))
		{
			MessageBox(
				theHWND,
				DXGetErrorDescription(result),
				DXGetErrorString(result),
				MB_OK | MB_ICONERROR);
			OutputDebugString("ERROR WITH PLAYING LOOPING SOUND");
			return false;
		}
		return true;
	}
	else if(!isLoop)
	{
		result = dsbuffer->Play(0, 0, 0);
		if(FAILED(result))
		{
			MessageBox(
				theHWND,
				DXGetErrorDescription(result),
				DXGetErrorString(result),
				MB_OK | MB_ICONERROR);
			OutputDebugString("ERROR WITH PLAYING THE SOUND");
			return false;
		}
		return true;
	}
}

bool Sound::StopTheSound()
{
	HRESULT result = dsbuffer->Stop();
	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH UNLOCKING");
		return false;
	}
	return true;
}
void Sound::ReleaseSoundStuff()
{
	if(dsbuffer)
	{
		dsbuffer->Release();
		dsbuffer = NULL;
	}
	if(ds8)
	{
		ds8->Release();
		ds8 = NULL;
	}
	// clears the vector "hard" (deallocated memory which .clear() doesn't)
	std::vector<char>().swap(dataBuffer);
}