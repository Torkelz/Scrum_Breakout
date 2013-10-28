#ifndef GLUTIL_H_
#define GLUTIL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
using namespace glm;
struct vertexVec
{
	float x,y,z;

	vertexVec(){x=y=z=0.0f;};
	vertexVec(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}
};


#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

const float MATH_EPS = 0.0001f;


#endif /* GLUTIL_H_ */