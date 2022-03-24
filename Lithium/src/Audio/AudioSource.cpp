#include "lipch.h"
#include "Audio/AudioSource.h"

#include "AL/al.h"
#include "AL/alext.h"
#include "alc/alcmain.h"
#include "alhelpers.h"

namespace Lithium
{

	AudioSource::~AudioSource()
	{
		alDeleteBuffers(1, &m_BufferHandle);
		alDeleteSources(1, &m_SourceHandle);
	}

	void AudioSource::SetPosition(float* pos)
	{

		alSourcefv(m_SourceHandle, AL_POSITION, pos);
	}

	void AudioSource::SetGain(float gain)
	{
		m_Gain = gain;

		alSourcef(m_SourceHandle, AL_GAIN, m_Gain);
	}

	void AudioSource::SetPitch(float pitch)
	{
		m_Pitch = pitch;

		alSourcef(m_SourceHandle, AL_PITCH, pitch);
	}

	void AudioSource::SetSpatial(bool spatial)
	{
		m_Spatial = spatial;

		alSourcei(m_SourceHandle, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	}

	void AudioSource::SetLoop(bool loop)
	{
		m_Loop = loop;
		alSourcei(m_SourceHandle, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	float AudioSource::GetGain()
	{
		return m_Gain;
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return std::pair<uint32_t, uint32_t>();
	}
	AudioSource::AudioSource(uint32_t handle, bool loaded, float length)
		: m_BufferHandle(handle), m_Loaded(loaded), m_TotalDuration(length)
	{
	}


}