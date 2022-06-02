#include "epch.h"
#include "AssetBrowserPanel.h"
#include "imgui/imgui.h"
#include "Core/Application.h"

namespace Lithium
{
	std::filesystem::path root = "assets";
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
		
		ImGui::Begin("Asset Browser");


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
		//ImGui::Separator();
		if (currentpath != root)
		{
			if (ImGui::Button("->"))
			{
				currentpath = currentpath.parent_path();
				Refresh();
			}
			ImGui::SameLine();
		}
		if (ImGui::Button("Refresh"))
		{
			Refresh();
		}
		ImGui::Separator();
		
		for (int i = 0;i < Cache.size();i++)
		{
			const auto& entry = Cache[i];
			if(!std::filesystem::exists(entry.path()))
			{
				continue;
			}
			auto path = entry.path();
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushID(path.filename().string().c_str());
		
			ImTextureID icontexid = entry.is_directory() ? (ImTextureID)_FolderIcon->GetID() : (ImTextureID)_FileIcon->GetID();
			ImGui::ImageButton(icontexid, { 60,60 }, { 0,1 }, { 1,0 });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (entry.is_directory())
				{
					currentpath /= path.filename();
					//LT_CORE_INFO((currentpath / path.filename()).string());
					Refresh();
				}
				if (path.extension() == ".png" || path.extension() == ".jpeg" || path.extension() == ".jpg")
				{
					AssetBrowserFileOpenEvent ev = AssetBrowserFileOpenEvent(Application::Get().assetManager->GetAssetFromPath<Ref<Texture>>(path.string()),Asset::AssetType::Texture);
					callback(ev);
				}
				if (path.extension() == ".anim")
				{
					EditAnimationEvent ev = EditAnimationEvent(Application::Get().assetManager->GetAssetFromPath<Ref<Animation>>(path.string()));
					callback(ev);
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, "assets");

				const wchar_t* itemPath;
				itemPath = (const wchar_t*)relativePath.c_str();
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

	void AssetBrowserPanel::OnProjectChange()
	{
		root = root / "assets";
		currentpath = root;
		Refresh();
	}

	void AssetBrowserPanel::Refresh()
	{
		Cache.clear();
		Cache.resize(0);
		for (auto entry : std::filesystem::directory_iterator(currentpath))
		{
			if (entry.path().extension() != ".metadata")
			{
				Cache.push_back(entry);
			}
		}
	}

}