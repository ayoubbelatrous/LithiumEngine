#pragma once
#include "Audio/AudioSource.h"

namespace Lithium
{
	class Audio
	{
	public:
		enum class AudioFileFormat
		{
			None = 0,
			Ogg,
			MP3
		};
		static Ref<AudioSource> LoadFromFile(const std::string& filename);
		static Ref<AudioSource> LoadAudioSourceOgg(const std::string& filename);
		static Ref<AudioSource> LoadAudioSourceMP3(const std::string& filename);
		static AudioFileFormat  GetAudioFileFormat(const std::string& filename);
		static void Init();
		static void Play(const Ref<AudioSource>& source);
		static void Stop(const Ref<AudioSource>& source);
	};
}