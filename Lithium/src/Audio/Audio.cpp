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
#define MINIMP3_IMPLEMENTATION
#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"

#include "vorbis/vorbisfile.h"
#include "vorbis/vorbisfile.h"

namespace Lithium
{
	static 	ALCdevice* s_AudioDevice = nullptr;
	static mp3dec_t s_Mp3d;
	static uint8_t* s_AudioScratchBuffer;
	static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024;


	Audio::AudioFileFormat Audio::GetAudioFileFormat(const std::string& filename)
	{
		std::filesystem::path path = filename;
		std::string extension = path.extension().string();

		if (extension == ".ogg")  return AudioFileFormat::Ogg;
		if (extension == ".mp3")  return AudioFileFormat::MP3;

		return AudioFileFormat::None;
	}
	static ALenum GetOpenALFormat(uint32_t channels)
	{
		// Note: sample size is always 2 bytes (16-bits) with
		// both the .mp3 and .ogg decoders that we're using
		switch (channels)
		{
		case 1:  return AL_FORMAT_MONO16;
		case 2:  return AL_FORMAT_STEREO16;
		}
		// assert
		return 0;
	}

	Ref<AudioSource> Audio::LoadFromFile(const std::string& filename)
	{
		if (GetAudioFileFormat(filename) == Audio::AudioFileFormat::MP3)
		{
			return LoadAudioSourceMP3(filename);

		}
		else if(GetAudioFileFormat(filename) == Audio::AudioFileFormat::Ogg)
		{
			return LoadAudioSourceOgg(filename);
		}
		else
		{
			ASSERT(GetAudioFileFormat(filename) != Audio::AudioFileFormat::None);
		}
	}

	Ref<AudioSource> Audio::LoadAudioSourceOgg(const std::string& filename)
	{
		FILE* f = fopen(filename.c_str(), "rb");

		OggVorbis_File vf;
		if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
			std::cout << "Could not open ogg stream\n";

		// Useful info
		vorbis_info* vi = ov_info(&vf, -1);
		auto sampleRate = vi->rate;
		auto channels = vi->channels;
		auto alFormat = GetOpenALFormat(channels);

		uint64_t samples = ov_pcm_total(&vf, -1);
		float trackLength = (float)samples / (float)sampleRate; // in seconds
		uint32_t bufferSize = 2 * channels * samples; // 2 bytes per sample (I'm guessing...)

		CORE_LOG("File Info - " << filename << ":");
		CORE_LOG("  Channels: " << channels);
		CORE_LOG("  Sample Rate: " << sampleRate);
		CORE_LOG("  Expected size: " << bufferSize);

		// TODO: Replace with Hazel::Buffer
		if (s_AudioScratchBufferSize < bufferSize)
		{
			s_AudioScratchBufferSize = bufferSize;
			delete[] s_AudioScratchBuffer;
			s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];
		}

		uint8_t* oggBuffer = s_AudioScratchBuffer;
		uint8_t* bufferPtr = oggBuffer;
		int eof = 0;
		while (!eof)
		{
			int current_section;
			long length = ov_read(&vf, (char*)bufferPtr, 4096, 0, 2, 1, &current_section);
			bufferPtr += length;
			if (length == 0)
			{
				eof = 1;
			}
			else if (length < 0)
			{
				if (length == OV_EBADLINK)
				{
					fprintf(stderr, "Corrupt bitstream section! Exiting.\n");
					exit(1);
				}
			}
		}

		uint32_t size = bufferPtr - oggBuffer;
		// assert bufferSize == size

		CORE_LOG("  Read " << size << " bytes");

		// Release file
		ov_clear(&vf);
		fclose(f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, oggBuffer, size, sampleRate);

		Ref<AudioSource> result = CreateRef<AudioSource>(buffer, true, trackLength);
		alGenSources(1, &result->m_SourceHandle);
		alSourcei(result->m_SourceHandle, AL_BUFFER, buffer);

		if (alGetError() != AL_NO_ERROR)
			CORE_LOG("Failed to setup sound source");

		return result;
	}
	Ref<AudioSource> Audio::LoadAudioSourceMP3(const std::string& filename)
	{
		mp3dec_file_info_t info;
		int loadResult = mp3dec_load(&s_Mp3d, filename.c_str(), &info, NULL, NULL);
		uint32_t size = info.samples * sizeof(mp3d_sample_t);

		auto sampleRate = info.hz;
		auto channels = info.channels;
		auto alFormat = GetOpenALFormat(channels);
		float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

		Ref<AudioSource> result = CreateRef<AudioSource>(buffer, true, lengthSeconds);
		alGenSources(1, &result->m_SourceHandle);
		alSourcei(result->m_SourceHandle, AL_BUFFER, buffer);

		CORE_LOG("File Info - " << filename << ":");
		CORE_LOG("  Channels: " << channels);
		CORE_LOG("  Sample Rate: " << sampleRate);
		CORE_LOG("  Size: " << size << " bytes");

		auto [mins, secs] = result->GetLengthMinutesAndSeconds();
		CORE_LOG("  Length: " << mins << "m" << secs << "s");
		
		if (alGetError() != AL_NO_ERROR)
			std::cout << "Failed to setup sound source" << std::endl;

		return result;
	}
	void Audio::Init()
	{
		if (InitAL(s_AudioDevice, nullptr, 0) != 0)
			std::cout << "Audio device error!\n";
		CORE_LOG("Audio Device Info:");
		CORE_LOG("  Name: " << s_AudioDevice->DeviceName);
		CORE_LOG("  Sample Rate: " << s_AudioDevice->Frequency);
		CORE_LOG("  Max Sources: " << s_AudioDevice->SourcesMax);
		CORE_LOG("    Mono: " << s_AudioDevice->NumMonoSources);
		CORE_LOG("    Stereo: " << s_AudioDevice->NumStereoSources);


		s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];

		// Init listener
		ALfloat listenerPos[] = { 0.0,0.0,0.0 };
		ALfloat listenerVel[] = { 0.0,0.0,0.0 };
		ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);

	}

	void Audio::Play(const Ref<AudioSource>& source)
	{
		alSourcePlay(source->m_SourceHandle);
	}

}