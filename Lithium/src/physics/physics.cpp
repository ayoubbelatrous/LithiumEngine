#include "lipch.h"
#include "physics/physics.h"
#include "physics/PhysicsTypes.h"
#include "box2d/box2d.h"

namespace Lithium
{

	PhysicsWorld::PhysicsWorld(glm::vec2 gravity)
	{
		m_PhysicsWorld = new b2World({ gravity.x,gravity.y });
	}

	PhysicsWorld::~PhysicsWorld()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	b2World* PhysicsWorld::GetPtr()
	{
		return m_PhysicsWorld;
	}

}