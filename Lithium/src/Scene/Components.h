#pragma once
#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include "Entity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include "AssetManager/TextureData.h"
#include "OrthographicCamera.h"
#include "Mesh/Mesh.h"


namespace Lithium
{
	enum class CameraMode
	{
		None, Orthographic, Perspective

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

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;

		glm::mat4 GetMatrix()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1), Position) * rotation * glm::scale(glm::mat4(1), Scale);
		}
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color;
		Ref<Texture> tex = nullptr;
		Ref<TextureData> textureData;
		glm::vec2 texIndex = glm::vec2();
		SpriteRendererComponent()
		{
			Color = glm::vec4(1);
			tex = CreateRef<Texture>();
			textureData = CreateRef<TextureData>(TextureMode::Single, 0, 0, 0, 0);
		}
		SpriteRendererComponent(const glm::vec4 color)
			:Color(color) {
			tex = CreateRef<Texture>();
			textureData = CreateRef<TextureData>(TextureMode::Single, 0, 0, 0, 0);

		}

		SpriteRendererComponent(const glm::vec4 color, Ref<TextureData> data)
			:Color(color) {
			tex = CreateRef<Texture>();
			textureData = data;
		}
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		glm::vec4 GetColor() const { return Color; }
		Ref<Texture> GetTexture() const { return tex; }
		Ref<TextureData> GetTextureData() const { return textureData; }
	};



	struct ChildManagerComponent
	{
		std::vector<entt::entity> Children;
		ChildManagerComponent() = default;
		ChildManagerComponent(const ChildManagerComponent& other) = default;


		void AddChild(Entity& entity)
		{
			Children.push_back(entity.GetHandle());
		}

	};


	struct CameraComponent
	{
		CameraComponent() = default;
		CameraMode mode;
		OrthographicCamera camera;
		const OrthographicCamera& GetCamera()
		{
			return camera;
		}
		CameraComponent(const CameraComponent& other) = default;

	};

	struct MeshComponent
	{
		MeshComponent() = default;
		Ref<Mesh> _Mesh;
		std::string path;
		MeshComponent(const MeshComponent& other) = default;
		Ref<Mesh> GetMesh()
		{
			return _Mesh;
		}
		void SetMesh(Ref<Mesh> mesh)
		{
			_Mesh = mesh;
		}
	};

	struct MeshRendererComponent
	{
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent& other) = default;
		
	};

	struct MaterialComponent
	{

	};

}

