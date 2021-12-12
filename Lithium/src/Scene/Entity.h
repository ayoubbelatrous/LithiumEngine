#pragma once
#include "Scene.h"
#include "entt.hpp"


namespace Lithium
{
	class Entity
	{
	public:
		Entity()
			:_Handle(entt::null), _Scene(nullptr)
		{

		}
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

		entt::entity GetHandle()
		{
			return _Handle;
		}
		Scene* getScene()
		{
			return _Scene;
		}


		template<typename T, typename... Args>
	    void AddOrReplaceComponent(Args&&... args)
		{
			_Scene->GetRegistry().emplace_or_replace<T>(_Handle, std::forward<Args>(args)...);
		}
	private:
		entt::entity _Handle;
		Scene* _Scene;
	};
}