///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>


// Direct Sound 可以用于加载任何格式的 audio( 甚至你自定义的 audio format ), 只不过需要你自己提供 audio format 的信息( 非常自由 )
// 用法: create a sound buffer with the play back format you would like, 将 audio 文件里的内容读到这个 buffer 里, 然后就可以播放了
// 原理: Direct Sound 实际上用了 primary buffer 和 secondary buffer 两个 buffer
// primary buffer 是用于播放的 main sound memory buffer, 它被创建在 default sound card 上
// secondary buffer 只是我们的 sound 从硬盘加载后存放的地方, 它被创建在 memory 里
// 因为我们可能同时播放好几个音频文件( 多个相互独立的 secondary buffer ), 实际播放时会先 mixing 所有 sound into the primary buffer
// 然后从 primary buffer 进行播放
class SoundClass
{
private:

	// the header of the .wav file
	// The header contains all the information about the audio file so we can use that to create a secondary buffer to accommodate the audio data
	struct WaveHeaderType
	{
		char chunkId[4];				// 4 bytes
		unsigned long chunkSize;		// 4 bytes
		char format[4];					// 4 bytes
		char subChunkId[4];				// 4 bytes
		unsigned long subChunkSize;
		unsigned short audioFormat;		// 2 bytes
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
};

#endif