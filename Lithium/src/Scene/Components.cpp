#include "lipch.h"
#include "Components.h"
#include "box2d/box2d.h"
#include "Core/Application.h"

namespace Lithium
{

	void Rigidbody2DComponent::SetVelocity(const glm::vec2 vel)
	{
		b2Body* body = (b2Body*)RuntimeBody;
		body->SetLinearVelocity({ vel.x,vel.y });
	}

	glm::vec2 Rigidbody2DComponent::GetVelocity()
		const
	{
		b2Body* body = (b2Body*)RuntimeBody;
		b2Vec2 vel = body->GetLinearVelocity();
		return glm::vec2({ vel.x, vel.y });
	}

	void ParticleSystemRenderer::OnUpdate(const glm::vec3& Position)
	{
		for (size_t i = 0; i < ParticlesPerFrame; i++)
		{

			ParticleProps tProps = Properties;
			tProps.Position += Position;
			pParticleSystem.Emit(tProps);
		}
		pParticleSystem.OnUpdate((float)Application::Get().GetDeltaTime());
	}

	void ButtonComponent::InvokeFunction()
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[BoundEntity], Application::Get().sceneManager->GetActiveScene().get());
		ScriptGroupeComponent& sgc = entity.GetComponent<ScriptGroupeComponent>();

		for (int i = 0; i < sgc.Scripts.size(); i++)
		{
			if (sgc.Scripts[i].Name.compare(BoundScript) == 0)
			{
				sgc.Scripts[i].Scriptobject->InvokeMethod(BoundFunction, nullptr);
			}
		}

	}

}