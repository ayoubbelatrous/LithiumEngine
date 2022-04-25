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
		
		TextureIndexKeyFrame(int textureIndex,float timestamp)
			:m_TextureIndex(textureIndex),TimeStamp(timestamp)
		{

		}
		
		void Execute(const UUID& EntityID)
		{
			entt::entity e =  Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[EntityID];
			Entity entity(e, Application::Get().sceneManager->GetActiveScene().get());
			entity.GetComponent<SpriteRendererComponent>().TextureIndex = m_TextureIndex;
		}
	
		float TimeStamp = 0;
	private:
		int m_TextureIndex = 0;
	};

	class SpriteColorKeyFrame
	{
	public:
		SpriteColorKeyFrame(glm::vec4 value, float timestamp)
			:m_Value(value), TimeStamp(timestamp)
		{

		}
		void Interpolate(const UUID& EntityID,glm::vec4 nextFrame,float currentTime)
		{
			entt::entity e = Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[EntityID];
			Entity entity(e, Application::Get().sceneManager->GetActiveScene().get());
			glm::vec4 finalcolor;
			
			float t = currentTime;

			
			finalcolor.r = m_Value.r * (1.f - t) + nextFrame.r * t;
			finalcolor.g = m_Value.g * (1.f - t) + nextFrame.g * t;
			finalcolor.b = m_Value.b * (1.f - t) + nextFrame.b * t;
			finalcolor.a = m_Value.a * (1.f - t) + nextFrame.a * t;

			entity.GetComponent<SpriteRendererComponent>().Color = finalcolor;
		}

		float TimeStamp = 0;
		glm::vec4 m_Value;
	private:
		
	};

	class AnimationTrack
	{
	public:
		virtual void Execute(const UUID& EntityID,float currentTime) = 0;
	};
	class TextureIndexTrack : public AnimationTrack
	{
	public:
		void PushKeyFrame(TextureIndexKeyFrame keyframe)
		{
			m_KeyFrames.push_back(keyframe);
		}

		virtual void Execute(const UUID& EntityID,float currentTime) override
		{
			for (size_t i = 0; i < m_KeyFrames.size(); i++)
			{
				if (m_KeyFrames[i].TimeStamp < currentTime)
				{
					m_KeyFrames[i].Execute(EntityID);
				}
			}
		}
	protected:
		std::vector<TextureIndexKeyFrame> m_KeyFrames;
	};

	class SpriteColorTrack : public AnimationTrack
	{
	public:
		void PushKeyFrame(SpriteColorKeyFrame keyframe)
		{

			m_KeyFrames.push_back(keyframe);
		}

		virtual void Execute(const UUID& EntityID, float currentTime) override
		{
			for (size_t i = 0; i < m_KeyFrames.size(); i++)
			{
				if (m_KeyFrames[i].TimeStamp < currentTime)
				{
					if (i >= m_KeyFrames.size())
					{
						m_KeyFrames[i].Interpolate(EntityID, m_KeyFrames[i + 1].m_Value, currentTime);
					}
					else
					{
						m_KeyFrames[i].Interpolate(EntityID, m_KeyFrames[0].m_Value, currentTime);

					}
				}
			}
		}
	protected:
		std::vector<SpriteColorKeyFrame> m_KeyFrames;
	};

	class Animation
	{
	public:
		void Update()
		{
			m_CurrentTime += Application::Get().GetDeltaTime();
			if (m_CurrentTime >= m_Duration)
			{
				if (m_Loop)
				{
					m_CurrentTime = 0.0f;
				}
				return;
			}
			for (size_t i = 0; i < m_Tracks.size(); i++)
			{
				m_Tracks[i]->Execute(EntityID, m_CurrentTime);
			}
		}

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
			for (int i = 0;i < m_Tracks.size();i++)
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