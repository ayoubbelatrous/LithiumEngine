#include "epch.h"
#include "imgui/imgui.h"

#include "ProjectWizard.h"
#include "Core/Base.h"
#include "Utils/WindowsPlatformUtils.h"


namespace Lithium
{
	static std::string ProjectName;
	static std::string ProjectPath;
	bool ProjectWizard::Update(bool* open,std::filesystem::path& path)
	{
		ImGui::Begin("Create Project",open);
		ImGui::Text("Create Project");
		char NameBuffer[256];
		memset(NameBuffer, 0, 256);
		std::strncpy(NameBuffer, ProjectName.c_str(),256);

		char PathBuffer[256];
		memset(PathBuffer, 0, 256);
		std::strncpy(PathBuffer, ProjectPath.c_str(), 256);

		if (ImGui::InputText("Name", NameBuffer, 256))
		{
			ProjectName = NameBuffer;
		}

		if (ImGui::InputText("Path", PathBuffer, 256))
		{
			ProjectPath = PathBuffer;

		}

		if (ImGui::Button("Explore"))
		{
			ProjectPath = FileDialogs::OpenFolder("");
		}

		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
	
			
			if (CreateProject(ProjectPath, ProjectName))
			{
				path = std::filesystem::path(ProjectPath) / ProjectName;
				ImGui::End();
				return true;
			}
			else
			{
				ImGui::End();
				return false;
			}

		}
		ImGui::End();
		return false;
	}

	bool ProjectWizard::CreateProject(const std::filesystem::path& path,const std::string& name)
	{
		const std::filesystem::path ProjectTemplatePath = "assets/Editor/templates/ProjectTemplate";
		if (std::filesystem::exists(path))
		{
			const auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
			std::filesystem::create_directory(path / name);
			std::filesystem::copy(ProjectTemplatePath, path / name, copyOptions);
			return true;
		}
		else
		{
			CORE_LOG("Project Path invalid!")
				return false;
		}
	}

}