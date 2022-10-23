#include "epch.h"
#include "Core/RecentProjectManager.h"

namespace Lithium::Core
{
	RecentProjectManager::RecentProjectManager()
	{
		m_RecentProjectsPath = std::filesystem::absolute(m_RecentProjectsPath).string();
		Load();
	}

	void RecentProjectManager::Load()
	{
		if (!std::filesystem::exists(m_RecentProjectsPath))
		{
			LT_CORE_ERROR("Editor :: recent projects config file was not found at: {}", m_RecentProjectsPath);
			return;
		}
		std::ifstream reader(m_RecentProjectsPath);
		std::string str;
		while (std::getline(reader,str))
		{
			m_RecentProjects.push_back(str);
		}
	}

	void RecentProjectManager::Save()
	{
		std::ofstream writer(m_RecentProjectsPath);

		for(auto path : m_RecentProjects)
		{
			writer << path << std::endl;
		}
	}
}