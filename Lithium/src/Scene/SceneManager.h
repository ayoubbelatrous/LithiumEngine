#pragma once
#include "Scene/Scene.h"

namespace Lithium
{
	class SceneManager
	{
	public:
		Ref<Scene> GetActiveScene()
		{
			return m_ActiveScene;
		}
		void SetActiveScene(const Ref<Scene>& scene)
		{
			m_ActiveScene = scene;
		}
		Ref<Scene> CreateScene()
		{
			Ref<Scene> scene = CreateRef<Scene>();
			m_SceneRegistry[scene->GetSceneID()] = scene;
			return m_SceneRegistry[scene->GetSceneID()];
		}
		void PushScene(const Ref<Scene>& scene)
		{
			m_SceneRegistry[scene->GetSceneID()] = scene;
		}
		void PopScene(uint64_t SceneID)
		{
			m_SceneRegistry.erase(SceneID);
		}
	private:
		std::map<uint64_t, Ref<Scene>> m_SceneRegistry;
		Ref<Scene> m_ActiveScene;
	};
}