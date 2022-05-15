#include "lipch.h"
#include "Animation/Animation.h"
#include "Core/Application.h"
#include "Scene/Entity.h"
#include "Utils/YamlUtils.h"

namespace Lithium
{
	void Animation::Update(const UUID& EntityID, float& Time)
	{
		Time += Application::Get().GetDeltaTime();
		if (Time >= m_Duration)
		{
			if (m_Loop)
			{
				Time = 0.0f;
			}
			return;
		}
		for (size_t i = 0; i < m_Tracks.size(); i++)
		{
			m_Tracks[i]->Step(EntityID, Time);
		}
	}


	void SpriteColorKeyFrame::Interpolate(const UUID& EntityID, glm::vec4 nextFrame, float currentTime)
	{
		entt::entity e = Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[EntityID];
		Entity entity(e, Application::Get().sceneManager->GetActiveScene().get());
		glm::vec4 finalcolor;

		float t = currentTime;


		finalcolor.r = Value.r * (1.f - t) + nextFrame.r * t;
		finalcolor.g = Value.g * (1.f - t) + nextFrame.g * t;
		finalcolor.b = Value.b * (1.f - t) + nextFrame.b * t;
		finalcolor.a = Value.a * (1.f - t) + nextFrame.a * t;

		entity.GetComponent<SpriteRendererComponent>().Color = finalcolor;
	}

	void TextureIndexKeyFrame::Execute(const UUID& EntityID)
	{
		entt::entity e = Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[EntityID];
		Entity entity(e, Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<SpriteRendererComponent>().TextureIndex = TextureIndex;
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
					m_KeyFrames[i].Interpolate(EntityID, m_KeyFrames[i + 1].Value, currentTime);
				}
				else
				{
					m_KeyFrames[i].Interpolate(EntityID, m_KeyFrames[0].Value, currentTime);

				}
			}
		}
	}

	void SpriteColorTrack::Serialize(YAML::Emitter& emitter)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Type" << YAML::Value << "SpriteColorTrack";
		emitter << YAML::Key << "KeyFrames" << YAML::Value << YAML::BeginSeq;

		for (int i = 0; i < m_KeyFrames.size(); i++)
		{
			auto KeyFrame = m_KeyFrames[i];
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "TimeStamp" << YAML::Value << KeyFrame.TimeStamp;
			emitter << YAML::Key << "Value" << YAML::Value << KeyFrame.Value;
			emitter << YAML::EndMap;
		}
		emitter << YAML::EndMap;
		emitter << YAML::EndSeq;
	}

	void SpriteColorTrack::Deserialize(const Ref<Animation>& animation)
	{

	}

	void TextureIndexTrack::Step(const UUID& EntityID, float currentTime)
	{
		for (size_t i = 0; i < m_KeyFrames.size() - 1; i++)
		{
			if (currentTime < m_KeyFrames[i + 1].TimeStamp)
			{
				m_KeyFrames[i + 1].Execute(EntityID);
				break;
			}
		}
	}

	void TextureIndexTrack::Serialize(YAML::Emitter& emitter)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Type" << YAML::Value << "TextureIndexTrack";
		emitter << YAML::Key << "KeyFrames" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < m_KeyFrames.size(); i++)
		{
			auto KeyFrame = m_KeyFrames[i];
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "TimeStamp" << YAML::Value << KeyFrame.TimeStamp;
			emitter << YAML::Key << "Value" << YAML::Value << KeyFrame.TextureIndex;
			emitter << YAML::EndMap;
		}
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
	}
	void TextureIndexTrack::Deserialize(const Ref<Animation>& animation)
	{

	}

	void TextureIndexTrack::PushKeyFrame(TextureIndexKeyFrame keyframe)
	{
		m_KeyFrames.push_back(keyframe);
	}

	std::string Animation::SerializeAnimation(const Ref<Animation>& animation)
	{
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Animation" << YAML::Value << "NONE";
		emitter << YAML::Key << "Tracks" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < animation->GetTracks().size(); i++)
		{
			auto Track = animation->GetTracks()[i];
			Track->Serialize(emitter);
		}
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
		return std::string(emitter.c_str());
	}

	Ref<Animation> Animation::DeserializeAnimation(const std::string& path)
	{
		Ref<Animation> animation =  CreateRef<Animation>();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			CORE_LOG("failed to load scene");
			return nullptr;
		}
		auto Tracks = data["Tracks"];
		for (auto track : Tracks)
		{
			if (strcmp(track["Type"].as<std::string>().c_str(), "TextureIndexTrack") == 0)
			{
				TextureIndexTrack* pTrack = new TextureIndexTrack();
				auto KeyFrames = track["KeyFrames"];
				for (auto keyframe : KeyFrames)
				{
					float TimeStamp = keyframe["TimeStamp"].as<float>();
					int Value = keyframe["Value"].as<int>();
					pTrack->PushKeyFrame(TextureIndexKeyFrame(Value, TimeStamp));
				}
				animation->PushTrack(pTrack);
			}
			if (strcmp(track["Type"].as<std::string>().c_str(), "SpriteColorTrack") == 0)
			{
				SpriteColorTrack* pTrack = new SpriteColorTrack();
				auto KeyFrames = track["KeyFrames"];
				for (auto keyframe : KeyFrames)
				{
					float TimeStamp = keyframe["TimeStamp"].as<float>();
					glm::vec4 Value = keyframe["Value"].as<glm::vec4>();
					pTrack->PushKeyFrame(SpriteColorKeyFrame(Value, TimeStamp));
				}
				animation->PushTrack(pTrack);

			}
		}
		return animation;
	}

}