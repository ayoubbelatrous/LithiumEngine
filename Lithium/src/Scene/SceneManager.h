#pragma once
#include "Scene/Scene.h"

namespace Lithium
{
	class SceneManager
	{
	public:
		SceneManager();
		Ref<Scene> GetActiveScene()
		{
			return m_ActiveScene;
		}
		void SetActiveScene(const Ref<Scene>& scene)
		{
			m_ActiveScene = scene;
		}

	private:
		Ref<Scene> m_ActiveScene;
	};
}