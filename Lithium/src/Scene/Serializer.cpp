#include "lipch.h"

#include "Serializer.h"
#include "entt.hpp"
#include "yaml-cpp/yaml.h"
#include "Entity.h"
#include "Components.h"
#include "entt.hpp"
#include "Core/Base.h"
#include "Scene.h"
namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}


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
			emitter << YAML::Key << "children" << YAML::Value << "ids go here";
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

	void Serializer::SerializeScene(const std::string& path)
	{
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entities";
		emitter << YAML::Value << YAML::BeginSeq;
		_Scene->GetRegistry().each([&](auto entity)
		{
				SerializeEntity(emitter, { entity,_Scene.get() });
		});
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
		std::ofstream output(path);
		output << emitter.c_str();
	}

	void Serializer::DeserializeScene(const std::string& path)
	{

		YAML::Node data;
	    try
	    {
			data = YAML::LoadFile(path);
	    }
	    catch (YAML::ParserException e)
	    {
			CORE_LOG("failed to load scene");
			return;
	    }


		auto entities = data["Entities"];
		if (entities)
		
		for (auto entity : entities)
		{
			Entity deserEntity;
			auto id = data["Entity"];

			CORE_LOG(id.as<uint32_t>());

			auto nameC = data["Name"];
			if (nameC)
			{
				CORE_LOG(nameC);
				std::string name = nameC.as<std::string>();
				deserEntity = _Scene->CreateEntity(name);
			}

			auto transform = data["Transform"];
			if (transform)
			{
				deserEntity.AddComponent<TransformComponent>();
				TransformComponent& tc = deserEntity.GetComponent<TransformComponent>();
				tc.Position = transform["Position"].as<glm::vec3>();
				tc.Rotation = transform["Rotation"].as<glm::vec3>();
				tc.Scale = transform["Scale"].as<glm::vec3>();

			}
			auto sprite = data["Sprite Renderer"];

			if (sprite)
			{
				deserEntity.AddComponent<SpriteRendererComponent>();
				SpriteRendererComponent& sp = deserEntity.GetComponent<SpriteRendererComponent>();
				glm::vec4 color = sprite["Color"].as<glm::vec4>();
				sp.Color = color;
			}
		}
	}

}