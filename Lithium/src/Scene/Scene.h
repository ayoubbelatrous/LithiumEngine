#pragma once
#include "entt.hpp"
#include <string>
namespace Lithium
{
	class Entity;
	class Scene
	{
	public:

		Scene();

		Entity CreateEntity(const std::string& name);
		
		entt::registry& GetRegistry() { return _Registry; }
	private:
		entt::registry _Registry;

	};

}