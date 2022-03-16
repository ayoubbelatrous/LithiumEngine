#include "lipch.h"

#include "Serializer.h"
#include "entt.hpp"
#include "yaml-cpp/yaml.h"
#include "Entity.h"
#include "Components.h"
#include "entt.hpp"
#include "Core/Base.h"
#include "Scene.h"
#include "Core/Application.h"
namespace YAML {
	
	template<>
	struct convert<glm::vec2>
	{
		
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return emitter;
	}
	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec3& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return emitter;
	}

	YAML::Emitter& operator <<(YAML::Emitter& emitter, const glm::vec2& vector)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << vector.x << vector.y << YAML::EndSeq;
		return emitter;
	}
	static void SerializeScript(YAML::Emitter& emitter,const ScriptComponent& script)
	{
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "ClassName" << YAML::Value << script.Name;
		emitter << YAML::Key << "Fields" << YAML::Value << YAML::BeginSeq;

		for (auto& field : script.Scriptobject->GetFields())
		{
			emitter << YAML::BeginMap;

			switch (field.second->GetType())
			{
			case (ScriptType::Int):
			{
				int val = field.second->GetLocalValue<int>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "INT";
				emitter << YAML::Key << "Value" << YAML::Value << val;
				break;
			}
			case (ScriptType::Float):
			{
				float val = 0;
				val = field.second->GetLocalValue<float>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "FLOAT";
				emitter << YAML::Key << "Value" << YAML::Value << val;
				break;
			}
			case (ScriptType::Vec2):
			{
				glm::vec2 val = glm::vec2(0);
				val = field.second->GetLocalValue<glm::vec2>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "VEC2";
				emitter << YAML::Key << "Value" << YAML::Value << val;
				break;
			}


			case (ScriptType::Vec3):
			{
				glm::vec3 val = glm::vec3(0);
				val = field.second->GetValue<glm::vec3>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "VEC3";
				emitter << YAML::Key << "Value" << YAML::Value << val;
				break;
			}


			case (ScriptType::Vec4):
			{
				glm::vec4 val = glm::vec4(0);
				val = field.second->GetValue<glm::vec4>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "VEC4";
				emitter << YAML::Key << "Value" << YAML::Value << val;
			}
			break;

			case (ScriptType::String):
			{

				std::string val = field.second->GetValue<std::string>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "STRING";
				emitter << YAML::Key << "Value" << YAML::Value << val;
				break;
			}

			case (ScriptType::Entity):
			{
				uint64_t val = field.second->GetValue<uint64_t>();
				uint64_t test = 2525252525252525252;
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "ENTITY";
				emitter << YAML::Key << "Value" << YAML::Value << val;

				break;
			}
			}
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
	}


	static void SerializeEntity(YAML::Emitter& emitter, Entity entity)
	{
		IDComponent& idc = entity.GetComponent<IDComponent>();

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entity" << YAML::Value << (uint64_t)idc.ID;
		NameComponent& name = entity.GetComponent<NameComponent>();
		emitter << YAML::Key << "Name" << YAML::Value << name.GetName();

		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		emitter << YAML::Key << "Transform" << YAML::BeginMap;
		emitter << YAML::Key << "Position" << YAML::Value << tc.Position;
		emitter << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
		emitter << YAML::Key << "Scale" << YAML::Value << tc.Scale;
		emitter << YAML::EndMap;


		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
			emitter << YAML::Key << "Sprite Renderer" << YAML::BeginMap;
			emitter << YAML::Key << "Color" << YAML::Value << sprite.Color;
			emitter << YAML::Key << "Texture Path" << YAML::Value << sprite.tex->GetPath();
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<RelationShipComponent>())
		{
			RelationShipComponent& cm = entity.GetComponent<RelationShipComponent>();
			emitter << YAML::Key << "Child Manager" << YAML::BeginMap;
			emitter << YAML::Key << "children" << YAML::Value << "ids go here";
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptGroupeComponent>())
		{
			//NOTE: use local value instead of mono value in scene
			ScriptGroupeComponent& ScriptGroupe = entity.GetComponent<ScriptGroupeComponent>();

			emitter << YAML::Key << "ScriptGroupe" << YAML::Value << YAML::BeginMap;

			emitter << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
			for (auto& scc:ScriptGroupe.Scripts)
			{
				
				SerializeScript(emitter, scc);
				
			}

			emitter << YAML::EndSeq;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			emitter << YAML::Key << "Rigidbody2D" << YAML::BeginMap;
			emitter << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			switch (rb2d.Type)
			{
			case (PhysicsBodyType::Static):
				{
				emitter << YAML::Key << "Type" << YAML::Value << "Static";
					break;
				}
			case (PhysicsBodyType::Dynamic):
			{
				emitter << YAML::Key << "Type" << YAML::Value << "Dynamic";

				break;
			}
			case (PhysicsBodyType::Kinematic):
			{
				emitter << YAML::Key << "Type" << YAML::Value << "Kinematic";

				break;
			}
			}
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

	void Serializer::SerializeScene(const std::string& path)
	{
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Scene" << YAML::Value << "NONE";
		emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
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

		for (auto entity : entities)
		{
			Entity deserEntity;

			uint64_t uuid = entity["Entity"].as<uint64_t>();
			std::string name = entity["Name"].as<std::string>();
			deserEntity = _Scene->CreateEntityWithUUID(UUID(uuid), name);

			auto transform = entity["Transform"];
			if (transform)
			{
				deserEntity.AddComponent<TransformComponent>();
				TransformComponent& tc = deserEntity.GetComponent<TransformComponent>();
				tc.Position = transform["Position"].as<glm::vec3>();
				tc.Rotation = transform["Rotation"].as<glm::vec3>();
				tc.Scale = transform["Scale"].as<glm::vec3>();

			}
			auto sprite = entity["Sprite Renderer"];

			if (sprite)
			{
				deserEntity.AddComponent<SpriteRendererComponent>();
				SpriteRendererComponent& sp = deserEntity.GetComponent<SpriteRendererComponent>();
				glm::vec4 color = sprite["Color"].as<glm::vec4>();
				std::string path = sprite["Texture Path"].as<std::string>();
				sp.Color = color;
				if (path != "")
				{
					sp.tex = CreateRef<Texture>(path);
				}
				else
				{
					sp.tex = CreateRef<Texture>();
				}

			}

			
			auto ScriptGroupe = entity["ScriptGroupe"];
			if (ScriptGroupe)
			{
				deserEntity.AddComponent<ScriptGroupeComponent>();
				ScriptGroupeComponent& scriptgroupe = deserEntity.GetComponent<ScriptGroupeComponent>();
				auto Scripts = ScriptGroupe["Scripts"];
				for (auto script : Scripts)
				{
					
					ScriptComponent& scc = ScriptComponent();
					std::string ScriptName = script["ClassName"].as<std::string>();
					scc.Name = ScriptName;
					scc.Scriptobject = Application::Get().Monoserver->GetObject(ScriptName);
					scc.Loaded = true;
					auto Fields = script["Fields"];

					for (auto field : Fields)
					{

						auto name = field["Name"].as<std::string>();
						std::string type = field["Type"].as<std::string>();

						//convert string to script type 

						ScriptType FieldType = ConvertStringToScriptType(type);

						//convert value to the proper type
						FieldValue Value;
						{
							switch (FieldType)
							{
							case(ScriptType::Int):
							{
								Value = field["Value"].as<int>();
								break;
							}
							case(ScriptType::Float):
							{
								Value = field["Value"].as<float>();
								break;
							}
							case(ScriptType::Vec2):
							{
								Value = field["Value"].as<glm::vec2>();

								break;
							}
							case(ScriptType::Vec3):
							{
								Value = field["Value"].as<glm::vec3>();

								break;
							}
							case(ScriptType::Vec4):
							{
								Value = field["Value"].as<glm::vec4>();

								break;
							}
							case(ScriptType::String):
							{
								Value = field["Value"].as<std::string>();
								break;
							}

							case(ScriptType::Entity):
							{
								Value = field["Value"].as<uint64_t>();
								break;
							}
							}



							auto& ScriptFields = scc.Scriptobject->GetFields();
							if (ScriptFields.find(name) != ScriptFields.end())
							{

								switch (FieldType)
								{
								case(ScriptType::Int):
								{
									ScriptFields[name]->SetValue<int>(std::get<int>(Value));

									break;
								}
								case(ScriptType::Float):
								{
									ScriptFields[name]->SetValue<float>(std::get<float>(Value));
									break;
								}
								case(ScriptType::Vec2):
								{
									ScriptFields[name]->SetValue<glm::vec2>(std::get<glm::vec2>(Value));


									break;
								}
								case(ScriptType::Vec3):
								{
									ScriptFields[name]->SetValue<glm::vec3>(std::get<glm::vec3>(Value));


									break;
								}
								case(ScriptType::Vec4):
								{
									ScriptFields[name]->SetValue<glm::vec4>(std::get<glm::vec4>(Value));


									break;
								}
								case(ScriptType::String):
								{
									ScriptFields[name]->SetValue<std::string>(std::get<std::string>(Value));

									break;
								}
								case(ScriptType::Entity):
								{
									ScriptFields[name]->SetValue<uint64_t>(std::get<uint64_t>(Value));

									break;
								}

								}
							}
						}

					}
				   scriptgroupe.AddScript(scc);
				}

			}

			auto rigidbody2d = entity["Rigidbody2D"];
			if (rigidbody2d)
			{
				deserEntity.AddComponent<Rigidbody2DComponent>();
				Rigidbody2DComponent& rb2d = deserEntity.GetComponent<Rigidbody2DComponent>();
				rb2d.FixedRotation = rigidbody2d["FixedRotation"].as<bool>();
				std::string type = transform["Type"].as<std::string>();
				if (strcmp("Static", type.c_str()) == 0)
				{
					rb2d.Type = PhysicsBodyType::Static;
				}

				if (strcmp("Dynamic", type.c_str()) == 0)
				{
					rb2d.Type = PhysicsBodyType::Dynamic;
				}

				if (strcmp("Kinematic", type.c_str()) == 0)
				{
					rb2d.Type = PhysicsBodyType::Kinematic;
				}
			}

		}
	}
}