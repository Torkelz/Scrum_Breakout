#ifndef GLUTIL_H_
#define GLUTIL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

const float PI       = 3.14159265358979323f;
const float TWOPI	 = 2 * PI;
const float MATH_EPS = 0.0001f;


#endif /* GLUTIL_H_ */
