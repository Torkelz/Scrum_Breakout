#pragma once

#include "Scene.h"
#include "d3dUtil.h"

class D3DTextDevice;
class Game;
class Menu;

using namespace DirectX;

class HighScore : public Scene
{
public:
	HighScore();
	HighScore(Game* p_pGame, Menu* p_pMenu);
	~HighScore();

	void init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight);
	void update();
	void draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster);

	void					leftMouseClick( vec2 p_mousePosition );
	void					rightMouseClick( vec2 p_mousePosition );
	void					mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	
	// Set pointers to scenes in Direct3D.
	void setMenu(Menu* p_pMenu);
	void setGame(Game* p_pGame);

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	D3DTextDevice*			m_pTextDevice;

	// Pointers to scenes in Direct3D.
	Game*					m_pGame;
	Menu*					m_pMenu;
};

