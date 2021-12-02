#pragma once
#include "entt.hpp"
namespace Lithium
{
	class Entity;
	class Scene
	{
	public:

		Scene();

		Entity CreateEntity();
		
		entt::registry& GetRegistry() { return _Registry; }
	private:
		entt::registry _Registry;

	};

}