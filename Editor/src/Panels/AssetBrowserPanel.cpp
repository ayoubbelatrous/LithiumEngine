#include "lipch.h"
#include "AssetBrowserPanel.h"
#include "imgui.h"

namespace Lithium
{
	extern const std::filesystem::path root = "assets";
	void AssetBrowserPanel::OnCreate()
	{
		_FolderIcon = CreateRef<Texture>("assets/Editor/icons/folder.png");
		 _FileIcon = CreateRef<Texture>("assets/Editor/icons/file.png");
		currentpath = root;
		Refresh();
	}

	void AssetBrowserPanel::OnUpdate()
	{

		float itemwidth = 78;
		
		ImGui::Begin("Project");


		float width = ImGui::GetContentRegionAvail().x;

		int columns = 1;
		if ((int)width < 10)
		{
			columns = 1;
		}
		else
		{
			columns = (int)(width / itemwidth);
		}


		if (columns < 1)
		{
			columns = 1;
		}
		ImGui::Columns(columns,0,false);
		if (currentpath != "assets")
		{
			if (ImGui::Button("->"))
			{
				currentpath = currentpath.parent_path();
				Refresh();
			}
		}
		
		for (auto entry: _Cache)
		{
		
			const auto& path = entry.path();
	

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushID(path.filename().string().c_str());
		
			ImTextureID icontexid = entry.is_directory() ? (ImTextureID)_FolderIcon->GetID() : (ImTextureID)_FileIcon->GetID();
			ImGui::ImageButton(icontexid, { 60,60 }, { 0,1 }, { 1,0 });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (entry.is_directory())
				{
					currentpath /= path.filename();
					Refresh();
				}
				if (path.extension() == ".png"|| path.extension() == ".jpg")
				{
					OpenSpriteEditorEvent ev = OpenSpriteEditorEvent(path.string());
					callback(ev);
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, "assets");

				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("ASSET_FILE", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Image(icontexid, { 50,50}, { 0,1 }, { 1,0 });
				ImGui::Text(relativePath.filename().string().c_str());
				ImGui::EndDragDropSource();
			}
			
			ImGui::PopStyleColor();
			ImGui::TextWrapped(path.filename().string().c_str());
			//ImGui::Text("%s",entry.path().string());
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::End();
	}

	void AssetBrowserPanel::Refresh()
	{
		
		_Cache.clear();
		for (auto& entry : std::filesystem::directory_iterator(currentpath))
		{
			if (entry.path().extension() != ".metadata")
			{
				_Cache.push_back(entry);
			}
		}
	}

}