#pragma once
#include <vector>
#include "glm.hpp"
#include "Core/Log.h"

namespace Lithium
{

	enum class KeyFrameType
	{
		Position
	};

	class KeyFrame
	{
	public:
		float TimeStamp = 0.0f;
		virtual KeyFrameType GetKeyFrameType() const = 0;
	};
#define KEYFRAME_TYPE(t) static KeyFrameType GetType() { return KeyFrameType::t; }\
								virtual KeyFrameType GetKeyFrameType() const override { return GetType(); }
	class PositionKeyFrame : public KeyFrame
	{
	public:
		PositionKeyFrame(glm::vec3& position) : Position(position){}
		glm::vec3& Position;
		KEYFRAME_TYPE(KeyFrameType::Position);
	};
	class Track
	{
	public:
		Track() = default;
		virtual ~Track() {}
		void Update(float time)
		{
			for (size_t i = 0; i < m_KeyFrames.size(); i++)
			{
				if (m_KeyFrames[i]->GetKeyFrameType() == KeyFrameType::Position)
				{
					PositionKeyFrame* positionKeyFrame = static_cast<PositionKeyFrame*>(m_KeyFrames[i]);
					if (positionKeyFrame->TimeStamp <= time)
					{
						LT_CORE_INFO("Animating ..");
					}
				}
			}
			
		}
	protected:
		float m_Time = 0.0f;
		std::vector<KeyFrame*> m_KeyFrames;
		uint64_t m_EntityID;
	};

	class Animation
	{
	public:
		Animation() = default;
		virtual ~Animation();
	private:
		float m_Time = 0.0f;
		std::vector<Track*> m_Tracks;
	};
}