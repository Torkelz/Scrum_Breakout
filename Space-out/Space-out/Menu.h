#pragma once

#include "Scene.h"
#include "d3dUtil.h"

class AABB;
class D3DTextDevice;
class Game;

using namespace DirectX;

class Menu : public Scene
{
public:
	Menu();
	Menu(Game* p_pGame);
	~Menu(void);

	void init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight);
	void update();
	void draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster);

	void					leftMouseClick( vec2 p_mousePosition );
	void					rightMouseClick( vec2 p_mousePosition );
	void					mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	AABB*					m_hitBox;
	D3DTextDevice*			m_pTextDevice;

	// Pointers to scenes in Direct3D.
	Game*					m_pGame;
};

