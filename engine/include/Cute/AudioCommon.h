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
}

// PCM:��8,16,24,32,float��ʽ���͵�������������
// һ�����������������������
struct AudioInfo
{
	uint samples;	// ��������
	uint channels;	// ���ж�������
	uint format;	// soundformat
	uint bits;		// ռ�ж��ٱ���λ
	uint rate;		// �����ʣ�һ���Ӳ�������
};

CUTE_NS_END

