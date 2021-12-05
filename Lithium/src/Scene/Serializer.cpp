#include "lipch.h"

#include "Serializer.h"
#include "entt.hpp"
#include "yaml-cpp/yaml.h"
#include "Entity.h"
#include "Components.h"
#include "entt.hpp"

namespace Lithium
{
	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec4& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z <<vector.w<< YAML::EndSeq;
		return emitter;
	}
	YAML::Emitter& operator <<(YAML::Emitter& emitter,const glm::vec3& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return emitter;
	}
	static void SerializeEntity(YAML::Emitter& emitter,Entity entity)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity.GetHandle();
		NameComponent& name = entity.GetComponent<NameComponent>();
		emitter << YAML::Key << "Name" << YAML::Value << name.GetName();

		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		emitter << YAML::Key << "Transform" << YAML::BeginMap;
		emitter << YAML::Key << "Position" << YAML::Value <<tc.Position;
		emitter << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
		emitter << YAML::Key << "Scale" << YAML::Value << tc.Scale;
		emitter << YAML::EndMap;


		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
			emitter << YAML::Key << "Sprite Renderer" << YAML::BeginMap;
			emitter << YAML::Key << "Color" << YAML::Value << sprite.Color;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<ChildManagerComponent>())
		{
			ChildManagerComponent& cm = entity.GetComponent<ChildManagerComponent>();
			emitter << YAML::Key << "Child Manager" << YAML::BeginMap;
			emitter << YAML::Key << "children" << YAML::Value << cm.Children.data();
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

	void Serializer::SerializeScene(const Ref<Scene>& scene,const std::string& path)
	{
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entities";
		emitter << YAML::Value << YAML::BeginSeq;
		scene->GetRegistry().each([&](auto entity)
		{
				SerializeEntity(emitter, { entity,scene.get() });
		});
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
		std::ofstream output(path);
		output << emitter.c_str();
	}

}