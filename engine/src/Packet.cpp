//! Server/Packet
#include "Cute/Packet.h"

CUTE_NS_BEGIN

Packet::Packet()
	: m_flag(0)
{
}

void Packet::setStatus(uint32 status)
{
	m_status = status;
	setMask(MASK_STATUS);
}

void Packet::setGID(uint32 gid)
{
	m_gid = gid;
	setMask(MASK_GID);
}

void Packet::setUID(uint64 uid)
{
	m_uid = uid;
	setMask(MASK_UID);
}

void Packet::reset()
{
	m_flag = 0;
}

CUTE_NS_END
