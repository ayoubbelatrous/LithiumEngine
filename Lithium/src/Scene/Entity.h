#pragma once
#include "Scene.h"
#include "entt.hpp"


namespace Lithium
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity,Scene* scene)
			:_Handle(entity),_Scene(scene)
		{}
		template<typename T>
		bool HasComponent() {
			return _Scene->GetRegistry().any_of<T>(_Handle);
		}
		template<typename T,typename... Args>
		void AddComponent(Args&&... args)
		{
			_Scene->GetRegistry().emplace<T>(_Handle, std::forward<Args>(args)...);

		}

		template<typename T>
		T& GetComponent()
		{
			return _Scene->GetRegistry().get<T>(_Handle);
		}
	private:
		entt::entity _Handle;
		Scene* _Scene;
	};
}