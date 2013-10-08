#include "Pad.h"

Pad::Pad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : Object(p_pPos, p_pColor, p_objectName)
{
	m_vertices.push_back(vec3(-5.0f, -1.0f, 0.0f));
	m_vertices.push_back(vec3(5.0f, -1.0f, 0.0f));
	m_vertices.push_back(vec3(-5.0f, 1.0f, 0.0f));
	m_vertices.push_back(vec3(5.0f, 1.0f, 0.0f));
	m_boundingVolume = new AABB(m_vertices.at(3), m_vertices.at(0), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_texturePath = new std::wstring(L"Picatures/sphere.png");
	m_mouseOffset = 0.f;
	m_scale = 1.0f;
	m_sticky = false;
}

Pad::~Pad(){}

void Pad::update(mat4 p_translate, mat4 p_rotation)
{
	m_realPosition = vec3(p_translate[3].x,p_translate[3].y,p_translate[3].z);
	mat4 scale = mat4(1.0f) * m_scale;
	scale[3].w = 1.0f;
	m_boundingVolume->updatePosition(scale,p_rotation,p_translate);
}

bool Pad::collide(BoundingVolume* p_pVolume)
{
	return ((AABB*)m_boundingVolume)->collide(p_pVolume);
}

void Pad::setPos(vec2 p_pos)
{
	m_mousePos = p_pos;
	m_mousePos.x *= -1;
	m_pos.x = p_pos.x*m_mouseOffset;
}

vec2 Pad::getMousePos()
{
	return m_mousePos;
}

void Pad::setMouseOffset(float p_x)
{
	m_mouseOffset = p_x;
}

vec3 Pad::getRealPosition()
{
	return m_realPosition;
}

void Pad::changeXCoordXAmount(float p_x)
{
	m_pos.x += p_x;
}

void Pad::bigger()
{
	m_scale *= 1.25f;
}

void Pad::smaller()
{
	m_scale *= 0.75;
}

float Pad::getScale()
{
	return m_scale;
}

bool Pad::getSticky()
{
	return m_sticky;
}

void Pad::setSticky(bool p_sticky)
{
	m_sticky = p_sticky;
}

vec3 Pad::getSavedVector()
{
	return m_savedVector;
}

void Pad::setSavedVector(vec3 p_savedVector)
{
	m_savedVector = p_savedVector;
}