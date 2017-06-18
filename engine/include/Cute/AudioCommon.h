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

// PCM:��8,16,24,32,float��ʽ���͵�������������
// һ�����������������������
struct AudioInfo
{
	uint format;	// soundformat
	uint samples;	// һ���ж��ٲ���num of samples
	uint channels;	// ���ж�������,num of channels
	uint rate;		// �����ʣ�һ����samples����(HZ) samples per second
	uint bits;		// ÿ��sampleռ�ж��ٱ���λ bit of one sample
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

