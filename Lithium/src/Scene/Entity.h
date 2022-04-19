#pragma once
#include "Scene/Scene.h"
#include "Scene/Components.h"

#include "entt.hpp"


namespace Lithium
{
	class Entity
	{
	public:
		Entity()
			:_Handle(entt::null), m_Scene(nullptr)
		{

		}
		Entity(entt::entity entity,Scene* scene)
			:_Handle(entity),m_Scene(scene)
		{}
		template<typename T>
		bool HasComponent() {
			return m_Scene->GetRegistry().any_of<T>(_Handle);
		}
		template<typename T,typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->GetRegistry().emplace<T>(_Handle, std::forward<Args>(args)...);

			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->GetRegistry().get<T>(_Handle);
		}
		template<typename T>
	    void RemoveComponent()
		{
			m_Scene->GetRegistry().remove<T>(_Handle);
		}
		entt::entity GetHandle()
		{
			return _Handle;
		}
		Scene* getScene()
		{
			return m_Scene;
		}


		template<typename T, typename... Args>
	    void AddOrReplaceComponent(Args&&... args)
		{
			m_Scene->GetRegistry().emplace_or_replace<T>(_Handle, std::forward<Args>(args)...);
		}

		explicit operator bool() const
		{
			if (_Handle == entt::null)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	private:
		entt::entity _Handle;
		Scene* m_Scene;
	};
}