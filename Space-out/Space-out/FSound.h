#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "fmod.hpp"
#include "fmod_errors.h"

#include <vector>

class FSound
{
private:
	static const int	m_nrofChannels = 5;
	FMOD::System*		m_pSystem;
	FMOD_RESULT			m_result;
	unsigned int		m_version;
	int					m_numDrivers;
	FMOD_SPEAKERMODE	m_speakerMode;
	FMOD_CAPS			m_caps;
	char				m_name[256];
	FMOD::Channel*		m_pChannels[m_nrofChannels];
	char				m_str[256];

	void ERRCHECK(FMOD_RESULT p_result);
	
public:
	FSound();
	~FSound();
	
	void init();

	void update();

	FMOD::Sound* loadSound(char* p_filename, bool p_loop);

	void play(FMOD::Sound* p_s, int p_id);

	unsigned int getSoundLength(FMOD::Sound* p_s);

	void setPause(bool p_p, int p_id);

	void setPauseAll(bool p_p);

	void setVolume(float p_vol, int p_id);

	bool getPaused(int p_id);

	bool isPlay(int p_id);
};

#endif