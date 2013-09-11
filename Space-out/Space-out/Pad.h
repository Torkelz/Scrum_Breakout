#ifndef PAD_H
#define PAD_H

#include <DirectXMath.h>
using namespace DirectX;

class Pad
{
private:
	XMFLOAT3 m_pos;

public:
	Pad();
	~Pad();

	void setPos(XMFLOAT3 p_pos);
	XMFLOAT3 getPos();
};

#endif