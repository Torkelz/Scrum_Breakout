#pragma once

#include "Scene.h"
#include "d3dUtil.h"
#include <string>
#include <sstream>
#include <iostream>

class D3DTextDevice;
class Game;
class Menu;

using namespace DirectX;

inline std::string IntToString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
};

struct HighScoreEntry
{
	int score;
	std::string name;
};

class HighScore : public Scene
{
public:
	HighScore();
	HighScore(Game* p_pGame, Menu* p_pMenu);
	~HighScore();

	void init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight);
	void update();
	void draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster, ID3D11BlendState* p_Blend);

	void					leftMouseClick( vec2 p_mousePosition );
	void					rightMouseClick( vec2 p_mousePosition );
	void					mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	
	// Set pointers to scenes in Direct3D.
	void setMenu(Menu* p_pMenu);
	void setGame(Game* p_pGame);
	void loadHighScoreListFromFile(std::string p_fileName);
	bool addHighScore(int p_score);
	void writeHighScoreToFile(std::string p_fileName);

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	D3DTextDevice*			m_pTextDevice;
	HighScoreEntry			m_scores[10];
	int						m_spacing;

	// Pointers to scenes in Direct3D.
	Game*					m_pGame;
	Menu*					m_pMenu;
};

