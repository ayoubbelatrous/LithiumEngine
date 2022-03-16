#pragma once


namespace Lithium
{
	enum class PhysicsBodyType
	{
		Static = 0,
		Dynamic,
		Kinematic,
	};

	enum class PhysicsColliderType
	{
		BoxCollider,
		CircleCollider,
    };

}