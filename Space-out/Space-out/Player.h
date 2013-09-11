#ifndef PLAYER_H
#define PLAYER_H

#include <DirectXMath.h>
using namespace DirectX;

class Player
{
private:
	XMFLOAT3 m_pos;

public:
	Player();
	~Player();

	void init();

	void update();

};

#endif