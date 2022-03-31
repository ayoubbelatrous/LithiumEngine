#pragma once
#include "imgui.h"

namespace Lithium
{
	class ProjectWizard
	{
	public:
		static bool Update(bool* open, std::filesystem::path& path);
		static bool CreateProject(const std::filesystem::path& path,const std::string& name);

	};
}