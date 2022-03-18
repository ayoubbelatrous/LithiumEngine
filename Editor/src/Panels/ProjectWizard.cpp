#include "lipch.h"
#include "ProjectWizard.h"


namespace Lithium
{
	void ProjectWizard::Update()
	{
		ImGui::Begin("Create Project");
		ImGui::Text("Create Project");
		char buffer[256];
		memset(buffer, 0, 256);
		ImGui::InputText("Name", buffer, 256);
		ImGui::End();
	}
}