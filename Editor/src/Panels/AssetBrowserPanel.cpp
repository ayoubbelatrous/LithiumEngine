#include "AssetBrowserPanel.h"
#include "imgui.h"
#include <filesystem>

namespace Lithium
{

	void AssetBrowserPanel::OnCreate()
	{
		_FolderIcon = CreateRef<Texture>("assets/Editor/icons/folder.png");
	}

	void AssetBrowserPanel::OnUpdate()
	{
		ImGui::Begin("Asset Browser");
		ImGui::Columns(5,0,false);
		for (auto& entry: std::filesystem::directory_iterator("assets"))
		{
			const auto& path = entry.path();
			auto relativePath = std::filesystem::relative(path, "assets");
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)_FolderIcon->GetID(), { 100,100}, { 0,1 }, { 1,0 });
			ImGui::PopStyleColor();
			ImGui::TextWrapped(relativePath.filename().string().c_str());
			//ImGui::Text("%s",entry.path().string());
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::End();
	}

}