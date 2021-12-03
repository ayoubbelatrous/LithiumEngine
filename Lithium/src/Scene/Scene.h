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
		void DeleteEntity(Entity entity);
		
		entt::registry& GetRegistry() { return _Registry; }

		void onEditorUpdate();
	private:
		entt::registry _Registry;

	};

}