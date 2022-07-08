#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include "Entity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/UUID.h"
#include "Script/ScriptObject.h"
#include "Camera/SceneCamera.h"
#include "Core/Math.h"
#include "physics/PhysicsTypes.h"
#include "AssetManager/Asset.h"
#include "Mesh/Mesh.h"
#include "Renderer/ParticleSystem.h"

namespace Lithium
{
	enum class CameraMode
	{
		None, Orthographic, Perspective

	};
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};
	struct NameComponent
	{
		std::string Name = "New Entity";
		NameComponent() = default;
		NameComponent(const std::string& name)
			:Name(name) {}
		NameComponent(const NameComponent& other) = default;
		std::string& GetName()
		{
			return Name;
		}
	};

	struct TransformComponent
	{

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale = glm::vec3(1);
		glm::mat4 ModelMatrix = glm::mat4(1);

		glm::vec3 LocalPosition = glm::vec3();
		glm::vec3 LocalRotation = glm::vec3();
		glm::vec3 LocalScale = glm::vec3();
		glm::mat4 GetMatrix()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
			return glm::translate(glm::mat4(1), Position) * rotation * glm::scale(glm::mat4(1), Scale);
		}

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color;
		Asset TextureAsset;
		int DrawOrder = 0;
		int TextureIndex = 0;
		SpriteRendererComponent()
		{
			Color = glm::vec4(1);
		}
		SpriteRendererComponent(const glm::vec4 color)
			:Color(color) {
		}

		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		glm::vec4 GetColor() const { return Color; }
	};



	struct RelationShipComponent
	{
		std::vector<UUID> Children;
		UUID Parent = 0;
		void AddChild(UUID id)
		{
			for (auto& uuid : Children)
			{
				if (id == uuid)
				{
					return;
				}
			}
			Children.push_back(id);
		}

		void RemoveChild(UUID id)
		{
			for (size_t i = 0; i < Children.size(); i++)
			{
				if (id == Children[i])
				{
					Children.erase(std::next(Children.begin(), i));
				}
			}
		}
		RelationShipComponent() = default;
		RelationShipComponent(const RelationShipComponent& other) = default;

	};


	struct Rigidbody2DComponent
	{


		bool FixedRotation = false;
		void* RuntimeBody = nullptr;
		bool Created = false;
		void SetVelocity(const glm::vec2 vel);
		glm::vec2 GetVelocity() const;
		PhysicsBodyType Type = PhysicsBodyType::Dynamic;


		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default;
	};


	struct BoxCollider2DComponent
	{
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		bool Created = false;
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		bool IsSensor = false;
		void* RuntimeFixture = nullptr;
	};

	struct CircleCollider2DComponent
	{
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
		bool Created = false;
		glm::vec2 Offset = {0,0};
		float Radius = 0.5f;
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		bool IsSensor = false;
		void* RuntimeFixture = nullptr;
	};
	struct ScriptComponent
	{
		Ref<ScriptObject> Scriptobject = nullptr;
		std::string Name;
		bool Loaded = false;
		bool Created = false;
		bool Valid = false;
		UUID uuid = 0;
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent(const std::string& name)
			:Name(name), uuid(UUID())
		{}
	};

	struct ScriptGroupeComponent
	{
		std::vector<ScriptComponent> Scripts;
		void AddScript(const std::string& name)
		{

			Scripts.emplace_back(name);
		}
		void AddScript(ScriptComponent& script)
		{
			Scripts.push_back(script);
		}
		ScriptGroupeComponent() = default;
		ScriptGroupeComponent(const ScriptGroupeComponent& other) = default;

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;
		glm::vec4 ClearColor = glm::vec4(0.0f);
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	struct AudioSourceComponent
	{
		Asset AudioAsset = Asset(UUID(0));
		bool PlayOnAwake = false;
		bool Loop = false;
		bool Spatial = false;
		float Gain = 1.0f;
		float Pitch = 1.0f;
		void SetLoop(bool loop)
		{
			Loop = loop;
		}
		void Play()
		{

		}
		void Stop()
		{
		}
		AudioSourceComponent() = default;
		AudioSourceComponent(const AudioSourceComponent&) = default;
	};

	struct AudioListenerComponent
	{
		AudioListenerComponent() = default;
		AudioListenerComponent(const AudioListenerComponent&) = default;
	};

	struct TextRenderer
	{
		std::string Text = "";
		glm::vec4 color = glm::vec4(1.0f);
		Asset FontAsset = Asset(UUID(0));
		float Spacing = 0.0f;
		float LineSpacing = 0.0f;
		bool ScreenSpace = true;

		TextRenderer() = default;
		TextRenderer(const TextRenderer&) = default;
	};

	struct AnimatorComponent
	{
		Asset AnimationAsset = Asset(0);

		void Play()
		{
			IsPlaying = true;
		}
		void Pause()
		{
			IsPlaying = false;
		}
		void Reset()
		{
			CurrentTime = 0.0f;
		}
		void SetLoop(bool loop)
		{
			Loop = loop;
		}
		bool IsPlaying;
		bool Loop;

		float CurrentTime = 0.0f;
		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent&) = default;
	};
	struct MeshRenderer
	{
		Asset ShaderAsset;
		std::vector<Ref<Mesh>> Meshes;
		bool hasModelFile;
		std::string ModelPath;
		//Material
	};

	struct ParticleSystemRenderer
	{
		ParticleSystem pParticleSystem;
		ParticleProps Properties;
		float ParticlesPerFrame = 5;
		void OnUpdate(const glm::vec3& Position);
		void OnRender()
		{
			pParticleSystem.OnRender();
		}
		ParticleSystemRenderer() = default;
		ParticleSystemRenderer(const ParticleSystemRenderer&) = default;
	};
	struct RectTrasnformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec2 Scale;
	};
	struct ButtonComponent
	{
		glm::vec4 Color = {1.0,1.0,1.0,1.0};
		glm::vec4 HoveredColor = { 0.8,0.8,0.8,1.0 };
		glm::vec4 PressColor = { 0.5,0.5,0.5,1.0 };
		glm::vec4 CurrentColor = { 1.0,1.0,1.0,1.0 };;

		std::string BoundFunction;
		bool CheckIntersection(const glm::vec2& Position, const glm::vec2& Min,const glm::vec2& Max)
		{
			bool intersected = Position.x > Min.x && Position.x < Max.x&&
				Position.y > Min.y && Position.y < Max.y;
			return intersected;
		}
	};

}

