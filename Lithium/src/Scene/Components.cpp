#include "lipch.h"
#include "Components.h"
#include "box2d/box2d.h"

namespace Lithium
{

	void Rigidbody2DComponent::SetVelocity(const glm::vec2 vel)
	{
		b2Body* body = (b2Body*)RuntimeBody;
		body->SetLinearVelocity({vel.x,vel.y});
	}

	glm::vec2 Rigidbody2DComponent::GetVelocity()
	const
	{
		b2Body* body = (b2Body*)RuntimeBody;
		b2Vec2 vel = body->GetLinearVelocity();
		return glm::vec2({ vel.x, vel.y});
	}

}