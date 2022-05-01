#include "epch.h"
#include "SpriteEditor.h"
#include "Core/Application.h"
#include "AssetManager/AssetManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"



namespace Lithium
{
	void SpriteEditor::OnUpdate(bool* open)
	{
		ImGui::Begin("Sprite Editor", open);
		TextureMetaData metadata = Application::Get().assetManager->GetAssetMetaData<TextureMetaData>(m_CurrentAsset);

		Ref<Texture> texture = Application::Get().assetManager->GetAsset<Ref<Texture>>(m_CurrentAsset);

		if (metadata.Mode == TextureMetaData::TextureMode::Multiple)
			if (ImGui::Button("Slice Options"))
				ImGui::OpenPopup("slice_options_popup");

		if (ImGui::BeginPopup("slice_options_popup"))
		{

			ImGui::InputInt2("Cell Size", (int*)glm::value_ptr(cellsize));

			if (ImGui::Button("Slice"))
			{
				metadata.CellsizeX = cellsize.x;
				metadata.CellsizeY = cellsize.y;
				Application::Get().assetManager->UpdateAssetMetaData<TextureMetaData>(metadata, m_CurrentAsset);
			}
			ImGui::EndPopup();
		}
		
		ImVec2 size = ImGui::GetContentRegionAvail();

		float aspect = 0;
		float width = 0;
		float height = 0;

		if (size.x >= size.y)
		{
			aspect = size.x / size.y;
			width = size.x / aspect;
			height = size.y;
		}
		else
		{
			aspect = size.y / size.x;
			width = size.x;
			height = size.y / aspect;
		}

	
		if (metadata.Mode == TextureMetaData::TextureMode::Multiple)
		{

			ImVec2 pos = ImGui::GetCursorPos();
			ImVec2 winpos = ImGui::GetWindowPos();
			ImVec4 color = { 0,1,0,1 };
			ImU32 col = ImColor(color);

			int twidth = texture->GetHeight();
			int theight = texture->GetHeight();

			int sizex = twidth / metadata.CellsizeX;
			int sizey = theight / metadata.CellsizeY;
			int cellsizeX = width / sizex;
			int cellsizeY = height / sizey;

			ImGui::Image(reinterpret_cast<void*>(texture->GetID()), { 2000,height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			for (size_t i = 0; i < (int)width / cellsizeX; i++)
			{
				ImDrawList* drawlist = ImGui::GetWindowDrawList();
				ImVec2 newpos = { winpos.x + pos.x ,winpos.y + pos.y + (cellsizeX * i) };
				ImVec2 lastpos = { winpos.x + pos.x + width ,winpos.y + pos.y + (cellsizeX * i) };
				drawlist->AddLine(newpos, lastpos, col, 1.5f);
			}

			for (size_t i = 0; i < (int)height / cellsizeY; i++)
			{
				ImDrawList* drawlist = ImGui::GetWindowDrawList();
				ImVec2 newpos = { winpos.x + pos.x + (cellsizeY * i) ,winpos.y + pos.y };
				ImVec2 lastpos = { winpos.x + pos.x + (cellsizeY * i),winpos.y + pos.y + height };
				drawlist->AddLine(newpos, lastpos, col, 1.5f);
			}
		}
		else
		{
			ImGui::Image(reinterpret_cast<void*>(texture->GetID()), { width,height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		ImGui::End();
	}
}
