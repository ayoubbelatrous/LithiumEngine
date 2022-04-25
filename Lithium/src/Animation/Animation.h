#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Core/Log.h"
#include "Core/UUID.h"
#include "Core/Application.h"
#include "Scene/Entity.h"

namespace Lithium
{
	class TextureIndexKeyFrame
	{
	public:

		TextureIndexKeyFrame(int textureIndex, float timestamp)
			:m_TextureIndex(textureIndex), TimeStamp(timestamp)
		{}

		void Execute(const UUID& EntityID);

		float TimeStamp = 0;
	private:
		int m_TextureIndex = 0;
	};


	class SpriteColorKeyFrame
	{
	public:
		SpriteColorKeyFrame(glm::vec4 value, float timestamp)
			:m_Value(value), TimeStamp(timestamp)
		{}

		void Interpolate(const UUID& EntityID, glm::vec4 nextFrame, float currentTime);

		float TimeStamp = 0;
		glm::vec4 m_Value;
	};

	class AnimationTrack
	{
	public:

		virtual void Step(const UUID& EntityID, float currentTime) = 0;
	};


	class TextureIndexTrack : public AnimationTrack
	{
	public:


		void PushKeyFrame(TextureIndexKeyFrame keyframe);
		virtual void Step(const UUID& EntityID, float currentTime) override;


	protected:
		std::vector<TextureIndexKeyFrame> m_KeyFrames;
	};

	class SpriteColorTrack : public AnimationTrack
	{
	public:


		void PushKeyFrame(SpriteColorKeyFrame keyframe);
		virtual void Step(const UUID& EntityID, float currentTime) override;


	protected:
		std::vector<SpriteColorKeyFrame> m_KeyFrames;
	};



	class Animation
	{
	public:

		void Update();

		void PushTrack(AnimationTrack* track)
		{
			m_Tracks.push_back(track);
		}
		const std::vector<AnimationTrack*> GetTracks() const
		{
			return m_Tracks;
		}
		void SetEntityUUID(const UUID& entityID)
		{
			EntityID = entityID;
		}
		void GetEntityUUID(const UUID& entityID)
		{
			EntityID = entityID;
		}
		void SetDuration(float duration)
		{
			m_Duration = duration;
		}
		float GetDuration()
		{
			return m_Duration;
		}
		void SetLoop(bool loop)
		{
			m_Loop = loop;
		}
		bool IsLoop()
		{
			return m_Loop;
		}
		~Animation()
		{
			for (int i = 0; i < m_Tracks.size(); i++)
			{
				delete m_Tracks[i];
			}
		}

	private:

		UUID EntityID;
		float m_CurrentTime = 0.0f;
		std::vector<AnimationTrack*> m_Tracks;
		float m_Duration = 0.0f;
		bool m_Loop = false;
	};
}