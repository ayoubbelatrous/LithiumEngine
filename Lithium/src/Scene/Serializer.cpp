#include "lipch.h"

#include "Serializer.h"
#include "entt.hpp"
#include "yaml-cpp/yaml.h"
#include "Utils/YamlUtils.h"
#include "Entity.h"
#include "Components.h"
#include "entt.hpp"
#include "Core/Base.h"
#include "Scene.h"
#include "Core/Application.h"

namespace Lithium
{
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

			case (ScriptType::Bool):
			{
				bool val = 0;
				val = field.second->GetLocalValue<bool>();
				emitter << YAML::Key << "Name" << YAML::Value << field.first;
				emitter << YAML::Key << "Type" << YAML::Value << "BOOL";
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

		emitter << YAML::Key << "LocalPosition" << YAML::Value << tc.LocalPosition;
		emitter << YAML::Key << "LocalRotation" << YAML::Value << tc.LocalRotation;
		emitter << YAML::Key << "LocalScale" << YAML::Value << tc.LocalScale;
		emitter << YAML::EndMap;


		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
			emitter << YAML::Key << "Sprite Renderer" << YAML::BeginMap;
			emitter << YAML::Key << "Color" << YAML::Value << sprite.Color;
			emitter << YAML::Key << "Texture ID" << YAML::Value << sprite.TextureAsset.GetUUID();
			emitter << YAML::Key << "DrawOrder" << YAML::Value << sprite.DrawOrder;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<RelationShipComponent>())
		{
			RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();
			emitter << YAML::Key << "RelationShipComponent" << YAML::BeginMap;
			emitter << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
			for (auto& id : rc.Children)
			{
				emitter << YAML::Key <<  (uint64_t)id;
			}

			emitter << YAML::EndSeq;
			emitter << YAML::Key << "Parent" << YAML::Value << (uint64_t)rc.Parent;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptGroupeComponent>())
		{
			//NOTE: use local value instead of mono value in scene
			ScriptGroupeComponent& ScriptGroupe = entity.GetComponent<ScriptGroupeComponent>();

			emitter << YAML::Key << "ScriptGroupe" << YAML::Value << YAML::BeginMap;

			emitter << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
			for (auto& scc : ScriptGroupe.Scripts)
			{
				if (scc.Loaded == true)
				{
					SerializeScript(emitter, scc);
				}
				
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

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			BoxCollider2DComponent& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			emitter << YAML::Key << "BoxCollider2D" << YAML::BeginMap;
			emitter << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			emitter << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			emitter << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			emitter << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			emitter << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;
			emitter << YAML::Key << "IsSensor" << YAML::Value << bc2d.IsSensor;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			CircleCollider2DComponent& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			emitter << YAML::Key << "CircleCollider2D" << YAML::BeginMap;
			emitter << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			emitter << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			emitter << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			emitter << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			emitter << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2d.RestitutionThreshold;
			emitter << YAML::Key << "IsSensor" << YAML::Value << cc2d.IsSensor;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			CameraComponent& camera = entity.GetComponent<CameraComponent>();
			emitter << YAML::Key << "CameraComponent" << YAML::BeginMap;
			emitter << YAML::Key << "OrhtographicSize" << YAML::Value << camera.Camera.GetOrthographicSize();
			emitter << YAML::Key << "NearPlane" << YAML::Value << camera.Camera.GetOrthographicNearClip();
			emitter << YAML::Key << "FarPlane" << YAML::Value << camera.Camera.GetOrthographicFarClip();
			emitter << YAML::Key << "Primary" << YAML::Value << camera.Primary;
			emitter << YAML::Key << "FixedAspectRatio" << YAML::Value << camera.FixedAspectRatio;			
			emitter << YAML::Key << "ClearColor" << YAML::Value << camera.ClearColor;			
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<AudioSourceComponent>())
		{
			AudioSourceComponent& audioSource = entity.GetComponent<AudioSourceComponent>();
			emitter << YAML::Key << "AudioSourceComponent" << YAML::BeginMap;
			emitter << YAML::Key << "AssetID" << YAML::Value << (uint64_t)audioSource.AudioAsset.GetUUID();
			emitter << YAML::Key << "PlayOnAwake" << YAML::Value << audioSource.PlayOnAwake;
			emitter << YAML::Key << "Loop" << YAML::Value << audioSource.Loop;
			emitter << YAML::Key << "Spatial" << YAML::Value << audioSource.Spatial;
			emitter << YAML::Key << "Gain" << YAML::Value << audioSource.Gain;
			emitter << YAML::Key << "Pitch" << YAML::Value << audioSource.Pitch;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<TextRenderer>())
		{
			TextRenderer& textrenderer = entity.GetComponent<TextRenderer>();
			emitter << YAML::Key << "TextRenderer" << YAML::BeginMap;
			emitter << YAML::Key << "FontAssetID" << YAML::Value << (uint64_t)textrenderer.FontAsset.GetUUID();
			emitter << YAML::Key << "Color" << YAML::Value << textrenderer.color;
			emitter << YAML::Key << "LineSpacing" << YAML::Value << textrenderer.LineSpacing;
			emitter << YAML::Key << "Spacing" << YAML::Value << textrenderer.Spacing;
			emitter << YAML::Key << "Text" << YAML::Value << textrenderer.Text;
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
				tc.LocalPosition = transform["LocalPosition"].as<glm::vec3>();
				tc.LocalRotation = transform["LocalRotation"].as<glm::vec3>();
				tc.LocalScale = transform["LocalScale"].as<glm::vec3>();

			}
		
			auto relationshipcomp = entity["RelationShipComponent"];
			if (relationshipcomp)
			{
				RelationShipComponent& rc = deserEntity.GetComponent<RelationShipComponent>();
				for (auto uuid : relationshipcomp["Children"])
				{
					rc.AddChild(uuid.as<uint64_t>());
				}
				rc.Parent = UUID(relationshipcomp["Parent"].as<uint64_t>());

			}
			auto sprite = entity["Sprite Renderer"];

			if (sprite)
			{
				deserEntity.AddComponent<SpriteRendererComponent>();
				SpriteRendererComponent& sp = deserEntity.GetComponent<SpriteRendererComponent>();
				glm::vec4 color = sprite["Color"].as<glm::vec4>();
				uint64_t TextureID = sprite["Texture ID"].as<uint64_t>();
				sp.TextureAsset = Asset(UUID(TextureID));
				sp.DrawOrder = sprite["DrawOrder"].as<int>();
				sp.Color = color;
			}

			
			auto ScriptGroupe = entity["ScriptGroupe"];
			if (ScriptGroupe)
			{
				deserEntity.AddComponent<ScriptGroupeComponent>();
				ScriptGroupeComponent& scriptgroupe = deserEntity.GetComponent<ScriptGroupeComponent>();
				auto Scripts = ScriptGroupe["Scripts"];
				for (auto script : Scripts)
				{
					
					ScriptComponent scc = ScriptComponent();
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

							case(ScriptType::Bool):
							{
								Value = field["Value"].as<bool>();
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

								case(ScriptType::Bool):
								{
									ScriptFields[name]->SetValue<bool>(std::get<bool>(Value));
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
				std::string type = rigidbody2d["Type"].as<std::string>();


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

			auto boxcollider2d = entity["BoxCollider2D"];
			if (boxcollider2d)
			{
				deserEntity.AddComponent<BoxCollider2DComponent>();
				BoxCollider2DComponent& bc2d = deserEntity.GetComponent<BoxCollider2DComponent>();
				bc2d.Size = boxcollider2d["Size"].as<glm::vec2>();
				bc2d.Offset = boxcollider2d["Offset"].as<glm::vec2>();
				bc2d.Density = boxcollider2d["Density"].as<float>();
				bc2d.Friction = boxcollider2d["Friction"].as<float>();
				bc2d.Restitution = boxcollider2d["Restitution"].as<float>();
				bc2d.RestitutionThreshold = boxcollider2d["RestitutionThreshold"].as<float>();
				bc2d.IsSensor = boxcollider2d["IsSensor"].as<bool>();

			}

			auto circleCollider2d = entity["CircleCollider2D"];
			if (circleCollider2d)
			{
				deserEntity.AddComponent<CircleCollider2DComponent>();
				CircleCollider2DComponent& cc2d = deserEntity.GetComponent<CircleCollider2DComponent>();
				cc2d.Radius = boxcollider2d["Radius"].as<float>();
				cc2d.Offset = boxcollider2d["Offset"].as<glm::vec2>();
				cc2d.Density = boxcollider2d["Density"].as<float>();
				cc2d.Friction = boxcollider2d["Friction"].as<float>();
				cc2d.Restitution = boxcollider2d["Restitution"].as<float>();
				cc2d.RestitutionThreshold = boxcollider2d["RestitutionThreshold"].as<float>();
				cc2d.IsSensor = boxcollider2d["IsSensor"].as<bool>();

			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				deserEntity.AddComponent<CameraComponent>();
				CameraComponent& camera = deserEntity.GetComponent<CameraComponent>();
				float orthosize = cameraComponent["OrhtographicSize"].as<float>();
				float nearplane = cameraComponent["NearPlane"].as<float>();
				float farplane = cameraComponent["FarPlane"].as<float>();
				bool primary = cameraComponent["Primary"].as<bool>();
				bool FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				camera.Camera.SetOrthographicSize(orthosize);
				camera.Camera.SetOrthographicNearClip(nearplane);
				camera.Camera.SetOrthographicFarClip(farplane);
				camera.Primary = primary;
				camera.FixedAspectRatio = FixedAspectRatio;
				camera.ClearColor = cameraComponent["ClearColor"].as<glm::vec4>();
			}

			auto audioSourceComponent = entity["AudioSourceComponent"];
			if (audioSourceComponent)
			{
				deserEntity.AddComponent<AudioSourceComponent>();
				AudioSourceComponent& audioSource = deserEntity.GetComponent<AudioSourceComponent>();
				audioSource.AudioAsset = Asset(UUID(audioSourceComponent["AssetID"].as<uint64_t>()));
				audioSource.PlayOnAwake = audioSourceComponent["PlayOnAwake"].as<bool>();
				audioSource.Loop = audioSourceComponent["Loop"].as<bool>();
				audioSource.Spatial = audioSourceComponent["Spatial"].as<bool>();
				audioSource.Gain = audioSourceComponent["Gain"].as<float>();
				audioSource.Pitch = audioSourceComponent["Pitch"].as<float>();

			}

			auto textRenderer = entity["TextRenderer"];
			if (textRenderer)
			{
				deserEntity.AddComponent<TextRenderer>();
				TextRenderer& textrendcomp = deserEntity.GetComponent<TextRenderer>();
				textrendcomp.FontAsset = Asset(UUID(textRenderer["FontAssetID"].as<uint64_t>()));
				textrendcomp.color = textRenderer["Color"].as<glm::vec4>();
				textrendcomp.LineSpacing = textRenderer["LineSpacing"].as<float>();
				textrendcomp.Spacing = textRenderer["Spacing"].as<float>();
				textrendcomp.Text = textRenderer["Text"].as <std::string> ();
			}

		}
	}
}