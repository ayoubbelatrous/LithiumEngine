#include "lipch.h"
#include "Animation/Animation.h"

namespace Lithium
{

	void Animation::Update()
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
			m_Tracks[i]->Step(EntityID, m_CurrentTime);
		}
	}

	void SpriteColorKeyFrame::Interpolate(const UUID& EntityID, glm::vec4 nextFrame, float currentTime)
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

	void TextureIndexKeyFrame::Execute(const UUID& EntityID)
	{
		entt::entity e = Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[EntityID];
		Entity entity(e, Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<SpriteRendererComponent>().TextureIndex = m_TextureIndex;
	}


	void SpriteColorTrack::PushKeyFrame(SpriteColorKeyFrame keyframe)
	{

		m_KeyFrames.push_back(keyframe);
	}

	void SpriteColorTrack::Step(const UUID& EntityID, float currentTime)
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

	void TextureIndexTrack::Step(const UUID& EntityID, float currentTime)
	{
		for (size_t i = 0; i < m_KeyFrames.size(); i++)
		{
			if (m_KeyFrames[i].TimeStamp < currentTime)
			{
				m_KeyFrames[i].Execute(EntityID);
			}
		}
	}

	void TextureIndexTrack::PushKeyFrame(TextureIndexKeyFrame keyframe)
	{
		m_KeyFrames.push_back(keyframe);
	}

}