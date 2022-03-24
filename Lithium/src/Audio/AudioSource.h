#pragma once

namespace Lithium
{
	class AudioSource
	{
	public:
		~AudioSource();
		AudioSource() = default;
		AudioSource(uint32_t handle, bool loaded, float length);
		bool IsLoaded() const { return m_Loaded; }

		void SetPosition(float x, float y, float z);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetSpatial(bool spatial);
		void SetLoop(bool loop);
		float GetGain();
		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;
	private:
		

		uint32_t m_BufferHandle = 0;
		uint32_t m_SourceHandle = 0;
		bool m_Loaded = false;
		bool m_Spatial = false;

		float m_TotalDuration = 0; // in seconds

		// Attributes
		float m_Position[3] = { 0.0f, 0.0f, 0.0f };
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		bool m_Loop = false;

		friend class Audio;

	};
}