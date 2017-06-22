//! Audio
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

enum SoundFormat
{
	SOUND_FORMAT_NONE,
	SOUND_FORMAT_PCM8,
	SOUND_FORMAT_PCM16,
	SOUND_FORMAT_PCM24,
	SOUND_FORMAT_PCM32,
	SOUND_FORMAT_PCMFLOAT,
	SOUND_FORMAT_BITSTREAM,
};

// PCM:分8,16,24,32,float格式，和单声道，多声道
// 一个采样包括：多个声道数据
struct AudioInfo
{
	uint format;	// soundformat
	uint samples;	// 一共有多少采用num of samples
	uint channels;	// 含有多少声道,num of channels
	uint rate;		// 采样率，一秒钟samples次数(HZ) samples per second
	uint bits;		// 每个sample占有多少比特位 bit of one sample
};

//struct AudioInfo
//{
//	uint format;
//	uint numSamples;
//	uint numChannels;
//	uint sampleRate;
//	uint bitDepth;
//};

CUTE_NS_END

