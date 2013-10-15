#pragma once
#include <glm.hpp>

using namespace glm;

struct SInitDataDifficulties
{
	float padStartSize;
	vec3 ballStartSpeed;
	int lives;
	float multiplier;
	float dropRateUp;
	float dropRateDown;

	SInitDataDifficulties(){}

	// padStartSize = p_size, ballStartSpeed = p_speed, ... , dropRateUp/Down = p_up/p_down
	SInitDataDifficulties(float p_size, vec3 p_speed, int p_lives, float p_mul, float p_up, float p_down)
	{
		padStartSize = p_size;
		ballStartSpeed = p_speed;
		lives = p_lives;
		multiplier = p_mul;
		dropRateUp = p_up;
		dropRateDown = p_down;
	}
};

class Difficulties
{
public:
	Difficulties();
	~Difficulties();

	void setInitValues(int p_difficulty);
	SInitDataDifficulties getDifficultyValues();
private:
	SInitDataDifficulties m_sData;
	


};

