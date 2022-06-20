#pragma once
#include "glm/glm.hpp"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "AssetManager/Asset.h"

namespace Lithium
{

	struct ParticleProps
	{
		glm::vec3 Position;
		glm::vec3 Velocity = {0,1.5,0};
		glm::vec3 VelocityVariation = { 0.5,5,0 };
		glm::vec4 ColorBegin = { 1,1,1,1 };
		glm::vec4 ColorEnd = {1,1,1,1};
		float SizeBegin = 1;
		float SizeEnd = 0.5;
		float SizeVariation = 1.5f;
		float LifeTime = 1.0f;
	};
	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();
		void Emit(const ParticleProps& props);
		void OnUpdate(float ts);
		void OnRender();
		void SetTextureAsset(Asset textureAsset);
		Asset GetTextureAsset();
	private:
		void Init();
		std::vector<Particle> m_ParticlePool;
		int m_PoolIndex = 999;

		Asset m_TextureAsset = 0;
	};
}

