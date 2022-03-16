#pragma once
#include "glm.hpp"

class b2World;
class b2ContactListener;
class b2Contact;

namespace Lithium
{
	
	class PhysicsWorld
	{
	public:
		PhysicsWorld(glm::vec2 gravity);
		~PhysicsWorld();

		b2World* GetPtr();
	private:
		b2World* m_PhysicsWorld = nullptr;
	};
}