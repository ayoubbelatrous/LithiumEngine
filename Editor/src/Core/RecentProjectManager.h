#pragma once

namespace Lithium::Core
{
	class RecentProjectManager
	{
	public:
		RecentProjectManager();

		std::vector<std::string> GetRecentProjects()
		{
			return m_RecentProjects;
		}

		void AddProject(std::string path)
		{
			for (auto project : m_RecentProjects)
			{
				if (project.compare(path) != 0)
				{
					m_RecentProjects.push_back(path);
				}
			}
			Save();
		}

	private:
		void Load();
		void Save();

		std::string m_RecentProjectsPath = "user/recentprojects.user";
		std::vector<std::string> m_RecentProjects;
	};
}