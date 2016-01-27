#pragma once
#include "Asset.h"

CU_NS_BEGIN

// ʱ����,����֡,�켣,��Ҫ֧�ֿհ�֡,����flash
// Ҫ��ÿ��frame��������ͬ�����ݽṹ
class CU_API AnimTrack : public Ref
{
public:
	AnimTrack();
	~AnimTrack();

	void addKeyFrame();

private:
	// just matrix?
	struct KeyFrame
	{
		bool fill;		// �Ƿ����֡
		uint frame;		// ��ǰ֡
		float* data;	// ����,���Ϊ�����ʾ�հ�֡
	};

	typedef Vector<KeyFrame> FrameVec;
	uint16	m_id;
	String	m_name;	// bone name
	String	m_type;
	String	m_prop;
	size_t	m_size;
	bool	m_quat;
	FrameVec m_frames;
};

// ʱ���Ἧ��
class CU_API AnimTrackSet : public Ref, public Vector<AnimTrack*>
{
public:
	AnimTrackSet(uint id) :m_id(id){}

	uint getID() const { return m_id; }
private:
	uint m_id;
};

enum WrapMode
{
	WM_ONCE,		// ����һ��
	WM_LOOP,		// ѭ������,������ʼ���²���
	WM_PINGPONG,	// ���ز���
	WM_CLAMP,		// ֹͣ�����һ֡
	WM_DEFAULT,		// �̳��ϼ�
};

// ������Ƭ,���clip���Թ���һ��AnimTrackSet,��ʱ���ʶ��ֹ
class CU_API AnimClip : public Ref
{
public:
	AnimClip(AnimTrackSet* tracks, const String& name, uint start, uint end, WrapMode mode = WM_ONCE);
	~AnimClip();

	const String& getName() const { return m_name; }
	uint getStart() const { return m_start; }
	uint getEnd() const { return m_end; }
	uint getLength() const { return m_length; }
	WrapMode getMode() const { return m_mode; }

private:
	AnimTrackSet* m_tracks;
	WrapMode m_mode;
	String	 m_name;
	uint	 m_start;
	uint	 m_end;
	uint	 m_length;
};

// ������Դ,���ж����Ƭ,������1��AnimTrackSet
class CU_API Animation : public Asset
{
	DECLARE_RTTI(Animation, Asset, "ANIM");
public:
	Animation();
	~Animation();

	bool load(Stream* stream);
	void save(Stream* stream);

	void addClip(AnimClip* clip);
	AnimClip* getClip(const String& name);

private:
	typedef Vector<AnimTrackSet*>	TracksVec;
	typedef Vector<AnimClip*>		ClipVec;
	ClipVec m_clips;
	TracksVec m_tracks;
};

CU_NS_END
