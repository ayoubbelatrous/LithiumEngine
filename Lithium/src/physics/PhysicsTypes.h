#pragma once


namespace Lithium
{
	namespace Physics
	{
		enum class BodyType
		{
			None = 0,
			Dynamic,
			Static
		};

		enum class ColliderType
		{
			None = 0,
			BoxCollider,
			CircleCollider,
		};
	}
	
}