#pragma once
#include "Asset.h"

CU_NS_BEGIN

// 时间轴,序列帧,轨迹,需要支持空白帧,类似flash
// 要求每个frame必须有相同的数据结构
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
		bool fill;		// 是否向后补帧
		uint frame;		// 当前帧
		float* data;	// 数据,如果为空则表示空白帧
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

// 时间轴集合
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
	WM_ONCE,		// 播放一次
	WM_LOOP,		// 循环播放,跳到开始重新播放
	WM_PINGPONG,	// 来回播放
	WM_CLAMP,		// 停止在最后一帧
	WM_DEFAULT,		// 继承上级
};

// 动画切片,多个clip可以共享一个AnimTrackSet,以时间标识起止
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

// 动画资源,含有多个切片,和至少1个AnimTrackSet
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
