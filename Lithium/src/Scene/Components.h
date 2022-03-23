#pragma once
#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Renderer/Texture.h"
#include "Mesh/Mesh.h"
#include "Core/Base.h"
#include "Entity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"
#include "Core/UUID.h"
#include "Script/ScriptObject.h"
#include "Camera/SceneCamera.h"
#include "Core/Math.h"
#include "Physics/PhysicsTypes.h"
#include "AssetManager/Asset.h"
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
			Children.push_back(id);
		}
		RelationShipComponent() = default;
		RelationShipComponent(const RelationShipComponent& other) = default;

	};


	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(const MeshComponent& other) = default;
		Ref<Mesh> _Mesh;
	};
	struct MeshRendererComponent
	{
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent& other) = default;
	};

	struct MaterialComponent
	{
		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent& other) = default;
		MaterialComponent(const Ref<Material>& mat)
			:material(mat)
		{}
		Ref<Material> material;
		
	};





	struct Rigidbody2DComponent
	{
		
		
		bool FixedRotation = false;
		void* RuntimeBody = nullptr;
		bool Created;
		void SetVelocity(const glm::vec2 vel);
		glm::vec2 GetVelocity();
		PhysicsBodyType Type = PhysicsBodyType::Dynamic;


		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default;
	};


	struct BoxCollider2DComponent
	{
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		bool Created;
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

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
			:Name(name),uuid(UUID())
		{}
	};

	struct ScriptGroupeComponent
	{
		std::vector<ScriptComponent> Scripts;
		void AddScript(const std::string& name)
		{
		
			Scripts.push_back(ScriptComponent(name));
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

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	struct AudioSourcePlayer
	{
		AudioSourcePlayer() = default;
		AudioSourcePlayer(const AudioSourcePlayer&) = default;
	};
}

