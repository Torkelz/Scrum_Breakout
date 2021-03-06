#include "FSound.h"


FSound::FSound()
{
	m_caps = 0;
	m_numDrivers = 0;
	m_pSystem = NULL;
	m_version = 0;
	m_result = FMOD_OK;
}

FSound::~FSound()
{
	m_pSystem->close();
	m_pSystem->release();
}


void FSound::init()
{

	//
	//	Create a System object and initialize.
	//
	m_result = FMOD::System_Create(&m_pSystem);
	ERRCHECK(m_result);

	m_result = m_pSystem->getVersion(&m_version);
	ERRCHECK(m_result);

	if(m_version < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %O8x. This program requires %O8\n", m_version, FMOD_VERSION);
		return;
	}
	m_result = m_pSystem->getNumDrivers(&m_numDrivers);
	ERRCHECK(m_result);

	if(m_numDrivers == 0)
	{
		m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(m_result);
	}
	else
	{
		m_result = m_pSystem->getDriverCaps(0, &m_caps, 0, &m_speakerMode);
		ERRCHECK(m_result);

		//
		//Set the user selected speaker mode.
		//
		m_result = m_pSystem->setSpeakerMode(m_speakerMode);
		ERRCHECK(m_result);

		if(m_caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			//
			//The user has the 'Acceleration' slider set to off! This is really bad
			//for latency! You might want to warn the user about this.
			// 
			m_result = m_pSystem->setDSPBufferSize(1024, 0);
			ERRCHECK(m_result);
		}

		m_result = m_pSystem->getDriverInfo(0, m_name, 256, 0);
		ERRCHECK(m_result);

		if(strstr(m_name, "SigmaTel"))
		{
			//
			//Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			//PCM floating point output seems to solve it.
			//
			m_result = m_pSystem->setSoftwareFormat(44000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(m_result);
		}
	}

	m_result = m_pSystem->init(100, FMOD_INIT_NORMAL, 0);
	if(m_result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		//Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		//back to stereo...
		m_result = m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		//ERRCHECK(m_result);
		//
		//... and re-init.
		//
		m_result = m_pSystem->init(100, FMOD_INIT_NORMAL, 0);
	}	
	ERRCHECK(m_result);
}




void FSound::ERRCHECK(FMOD_RESULT p_result)
{
    if (p_result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", p_result, FMOD_ErrorString(p_result));
        exit(-1);
    }
}


FMOD::Sound* FSound::loadSound(char* p_pFilename, bool p_loop)
{
	FMOD::Sound* sound;
	m_result = m_pSystem->createSound(p_pFilename, FMOD_HARDWARE, 0, &sound);
	ERRCHECK(m_result);

	if(p_loop)
		m_result = sound->setMode(FMOD_LOOP_NORMAL);
	else
		m_result = sound->setMode(FMOD_LOOP_OFF);
	ERRCHECK(m_result);
	return sound;
}


void FSound::play(FMOD::Sound* p_pSound, int p_id)
{
	m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, p_pSound, false, &m_pChannels[p_id]);
}

void FSound::update()
{
	m_pSystem->update();
}

unsigned int FSound::getSoundLength(FMOD::Sound* p_pSound)
{
	unsigned int length = 0;
	p_pSound->getLength(&length, FMOD_TIMEUNIT_MS);

	return length;
}

void FSound::setVolume(float p_vol, int p_id)
{
	if(p_id < m_nrofChannels)
		m_pChannels[p_id]->setVolume(p_vol);
}

void FSound::setPause(bool p_pause, int p_id)
{
	if(p_id < m_nrofChannels)
		m_pChannels[p_id]->setPaused(p_pause);
}

void FSound::setPauseAll(bool p_pause)
{
	for(int i = 0; i < m_nrofChannels; i++)
	{
		m_pChannels[i]->setPaused(p_pause);
	}
}

bool FSound::getPaused(int p_id)
{
	if(p_id < m_nrofChannels)
	{
		bool p;
		m_pChannels[p_id]->getPaused(&p);
		return p;
	}
	else return false;
}

bool FSound::isPlay(int p_id)
{
	bool play;
	m_pChannels[p_id]->isPlaying(&play);
	return play;
}