#include "lipch.h"

#include "Audio/Audio.h"
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <thread>
#include <filesystem>

#include "AL/al.h"
#include "AL/alext.h"
#include "alc/alcmain.h"
#include "alhelpers.h"

namespace Lithium
{
	static 	ALCdevice* s_AudioDevice = nullptr;

	void Audio::Test()
	{

		if (InitAL(s_AudioDevice, nullptr, 0) != 0)
			std::cout << "Audio device error!\n";
		CORE_LOG("Audio Device Info:");
		CORE_LOG("  Name: " << s_AudioDevice->DeviceName);
		CORE_LOG("  Sample Rate: " << s_AudioDevice->Frequency);
		CORE_LOG("  Max Sources: " << s_AudioDevice->SourcesMax);
		CORE_LOG("    Mono: " << s_AudioDevice->NumMonoSources);
		CORE_LOG("    Stereo: " << s_AudioDevice->NumStereoSources);

		// Init listener
		ALfloat listenerPos[] = { 0.0,0.0,0.0 };
		ALfloat listenerVel[] = { 0.0,0.0,0.0 };
		ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

}