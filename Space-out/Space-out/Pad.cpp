#include "Pad.h"

Pad::Pad()
{

}

Pad::~Pad()
{

}

void Pad::setPos(XMFLOAT3 p_pos)
{
	m_pos = p_pos;
}

XMFLOAT3 Pad::getPos()
{
	return m_pos;
}