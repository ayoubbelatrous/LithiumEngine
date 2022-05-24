#pragma once
#include "glm/glm.hpp"
#include "Core/UUID.h"
#include "Core/Base.h"
#include <vector>

namespace YAML
{
	class Emitter;
	class Node;
}

namespace Lithium
{
	enum class AnimationType
	{
		TextureIndex, SpriteColor
	};
#define ANIMATION_TYPE(t) static AnimationType GetType() { return AnimationType::t; }\
								virtual AnimationType GetAnimationType() const override { return GetType(); }
	class Animation;
	class TextureIndexKeyFrame
	{
	public:
		TextureIndexKeyFrame() = default;

		TextureIndexKeyFrame(int textureIndex, float timestamp)
			:TextureIndex(textureIndex), TimeStamp(timestamp)
		{}

		void Execute(const UUID& EntityID);

		float TimeStamp = 0;
		int TextureIndex = 0;
	};


	class SpriteColorKeyFrame
	{
	public:
		SpriteColorKeyFrame() = default;
		SpriteColorKeyFrame(glm::vec4 value, float timestamp)
			:Value(value), TimeStamp(timestamp)
		{}

		void Interpolate(const UUID& EntityID, glm::vec4 nextFrame, float currentTime);

		float TimeStamp = 0;
		glm::vec4 Value;
	};

	class AnimationTrack
	{
	public:
		AnimationTrack() = default;
		virtual void Step(const UUID& EntityID, float currentTime) = 0;
		virtual void Serialize(YAML::Emitter& emitter) = 0;
		virtual void Deserialize(const Ref<Animation>& animation) = 0;
		virtual AnimationType GetAnimationType() const = 0;

	};


	class TextureIndexTrack : public AnimationTrack
	{
	public:
		TextureIndexTrack() = default;


		void PushKeyFrame(TextureIndexKeyFrame keyframe);
		virtual void Step(const UUID& EntityID, float currentTime) override;
		virtual void Serialize(YAML::Emitter& emitter) override;
		virtual void Deserialize(const Ref<Animation>& animation) override;
		std::vector<TextureIndexKeyFrame> GetKeyFrames() {
			return m_KeyFrames;
		}
		ANIMATION_TYPE(TextureIndex);
	protected:
		std::vector<TextureIndexKeyFrame> m_KeyFrames;
	};

	class SpriteColorTrack : public AnimationTrack
	{
	public:
		SpriteColorTrack() = default;

		void PushKeyFrame(SpriteColorKeyFrame keyframe);
		virtual void Step(const UUID& EntityID, float currentTime) override;
		virtual void Serialize(YAML::Emitter& emitter) override;
		virtual void Deserialize(const Ref<Animation>& animation) override;
		ANIMATION_TYPE(SpriteColor);

	protected:
		std::vector<SpriteColorKeyFrame> m_KeyFrames;
	};



	class Animation
	{
	public:

		Animation() = default;
		void Update(const UUID& EntityID, float& Time);

		void PushTrack(AnimationTrack* track)
		{
			m_Tracks.push_back(track);
		}
		const std::vector<AnimationTrack*> GetTracks() const
		{
			return m_Tracks;
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

		static std::string SerializeAnimation(const Ref<Animation>& animation);
		static Ref<Animation> DeserializeAnimation(const std::string& path);

		~Animation()
		{
			for (int i = 0; i < m_Tracks.size(); i++)
			{
				delete m_Tracks[i];
			}
		}

	private:

		std::vector<AnimationTrack*> m_Tracks;
		float m_Duration = 0.0f;
		bool m_Loop = true;
	};
}