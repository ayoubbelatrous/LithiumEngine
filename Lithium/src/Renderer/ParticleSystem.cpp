#include "lipch.h"
#include "ParticleSystem.h"
#include "imgui/imgui.h"
#include "Core/Math.h"
#include <Renderer/Renderer2D.h>
#include <Core/Application.h>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Lithium
{

	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
		Init();
	}


	void ParticleSystem::Init()
	{

	}
	void ParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = props.Position;
		particle.Rotation = Math::Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (Math::Random::Float() - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (Math::Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.LifeTime = props.LifeTime;
		particle.LifeRemaining = props.LifeTime;
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Math::Random::Float() - 0.5f);
		particle.SizeEnd = props.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

	void ParticleSystem::OnUpdate(float ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender()
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
			if (m_TextureAsset.GetUUID() == 0)
			{
				BatchRenderer::DrawQuad(transform, color, -1);
			}
			else
			{
				Ref<Texture> texture = Application::Get().assetManager->GetAsset<Ref<Texture>>(m_TextureAsset);
				BatchRenderer::DrawQuad(transform, color, texture, -1);
			}

		}
	}

	void ParticleSystem::SetTextureAsset(Asset textureAsset)
	{
		m_TextureAsset = textureAsset;
	}

	Asset ParticleSystem::GetTextureAsset()
	{
		return m_TextureAsset;
	}

	ParticleSystem::~ParticleSystem()
	{

	}

}

