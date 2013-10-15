#include "Difficulties.h"


Difficulties::Difficulties()
{}


Difficulties::~Difficulties()
{}

void Difficulties::setInitValues(int p_difficulty)
{
	switch (p_difficulty)
	{
	case 0: // EASY Difficulty
		m_sData = SInitDataDifficulties(1.4f, vec3(10.f, -20.0f, 0.0f), 5, 0.75f, 0.7f);
		break;

	case 1: // NORMAL Difficulty
		m_sData = SInitDataDifficulties(1.2f, vec3(10.f, -40.0f, 0.0f), 4, 1.0f, 0.6f);
		break;

	case 2: // HARD Difficulty
		m_sData = SInitDataDifficulties(1.0f, vec3(10.f, -60.0f, 0.0f), 2, 1.5, 0.4f);
		break;

	default:
		break;
	}
}

SInitDataDifficulties Difficulties::getDifficultyValues()
{
	return m_sData;
}
