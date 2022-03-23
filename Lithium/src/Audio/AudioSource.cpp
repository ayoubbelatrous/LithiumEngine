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

	}

	void AudioSource::SetPosition(float x, float y, float z)
	{

	}

	void AudioSource::SetGain(float gain)
	{
		m_Gain = gain;

		alSourcef(m_SourceHandle, AL_GAIN, m_Gain);
	}

	void AudioSource::SetPitch(float pitch)
	{

	}

	void AudioSource::SetSpatial(bool spatial)
	{
		m_Spatial = spatial;

		alSourcei(m_SourceHandle, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	}

	void AudioSource::SetLoop(bool loop)
	{

	}

	float AudioSource::GetGain()
	{
		return m_Gain;
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return std::pair<uint32_t, uint32_t>();
	}

	AudioSource AudioSource::LoadFromFile(const std::string& file, bool spatial /*= false*/)
	{
		return AudioSource();
	}

	AudioSource::AudioSource(uint32_t handle, bool loaded, float length)
		: m_BufferHandle(handle), m_Loaded(loaded), m_TotalDuration(length)
	{
	}


}