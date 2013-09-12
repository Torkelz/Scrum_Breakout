#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2
{
	float x;
	float y;

	Vector2(){}

	Vector2(float p_x, float p_y)
	{
		x = p_x;
		y = p_y;
	}
};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(){}

	Vector3(float p_x, float p_y, float p_z)
	{
		x = p_x;
		y = p_y;
		z = p_z;
	}
};

#endif VECTOR2_H