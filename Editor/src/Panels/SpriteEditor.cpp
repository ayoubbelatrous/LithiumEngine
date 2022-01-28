#include "SpriteEditor.h"
#include "imgui.h"
#include <filesystem>
#include "gtc/type_ptr.hpp"
#include <iostream>
namespace Lithium
{
	extern AssetMananger assetManager;
	extern std::filesystem::path root;
	void SpriteEditor::OnCreate()
	{
		cellsize = glm::vec2(0);
	}

	void SpriteEditor::SetTexture(const Ref<Texture>& texture)
	{
		_Texture = texture;
		std::filesystem::path path(texture->GetPath());
		path.replace_extension(".metadata");
		if (std::filesystem::exists(path))
		{
			hasMetadata = true;
			_TextureData = assetManager.LoadTextureMetadata(path);
		}
		else
		{
			hasMetadata = false;
			_TextureData = CreateRef<TextureData>();
		}



	}

	void SpriteEditor::OnUpdate()
	{
		if (!_Open)
			return;

		ImGui::Begin("Sprite Editor", &_Open);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.25,0.25,0.25,1 });
		float width = ImGui::GetContentRegionAvailWidth();

		if (ImGui::Button("Slice"))
		{
			ImGui::OpenPopup("slice_options_popup");
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			Ref<TextureData> data = CreateRef<TextureData>(TextureMode::Single,
				cellsize.x, cellsize.y,
				_Texture->GetWidth(), _Texture->GetHeight());
			_TextureData = data;
			hasMetadata = false;
			std::filesystem::path path(_Texture->GetPath());
			path.replace_extension(".metadata");
			assetManager.GenerateTextureMetadata(data, path);
			MetaDataChangeEventEditorEvent ev(_Texture->GetPath());
			callback(ev);
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(width - 10);
		if (ImGui::Button("X", { 25,25 }))
		{
			Close();
		}
		ImGui::PopStyleColor();
		if (ImGui::BeginPopup("slice_options_popup"))
		{
			/*if (ImGui::MenuItem("ByCount"))
			{
				ImGui::InputInt2("Cell Size", (int*)glm::value_ptr(cellsize));
				if (ImGui::Button("Slice"))
				{
					Ref<TextureData> data = CreateRef<TextureData>(TextureMode::Single,
						cellsize.x, cellsize.y,
						_Texture->GetWidth(), _Texture->GetWidth());
					data->SliceByCount(cellsize.x, cellsize.y);
					_TextureData = data;
					hasMetadata = true;
					CORE_LOG(_Texture->GetPath());
					std::filesystem::path path(_Texture->GetPath());
					path.replace_extension(".metadata");
					assetManager.GenerateTextureMetadata(data, path);
				}
			}*/

			ImGui::InputFloat2("Cell Size", glm::value_ptr(cellsize));

			if (ImGui::Button("Slice"))
			{
			
				if (cellsize.x != 0 || cellsize.y != 0)
				{

				
				Ref<TextureData> data = CreateRef<TextureData>(TextureMode::Multiple,
					cellsize.x, cellsize.y,
					_Texture->GetWidth(), _Texture->GetHeight());
				_TextureData = data;
				hasMetadata = true;
				std::filesystem::path path(_Texture->GetPath());
				path.replace_extension(".metadata");
				assetManager.GenerateTextureMetadata(data, path);
				MetaDataChangeEventEditorEvent ev(_Texture->GetPath());
				callback(ev);
				}

			}
			ImGui::EndPopup();
		}
		if (_Texture)
		{
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
			if (hasMetadata)
			{
			
				ImVec2 pos = ImGui::GetCursorPos();
				ImVec2 winpos = ImGui::GetWindowPos();
				ImVec4 color = { 0,1,0,1 };
				ImU32 col = ImColor(color);
				int twidth = _TextureData->GetWidth();
				int theight = _TextureData->GetHeight();

				int sizex = twidth / _TextureData->GetCellSizeX();
				int sizey = theight / _TextureData->GetCellSizeY();
				int cellsizeX = width / sizex;
				int cellsizeY = height / sizey;
				
				ImGui::Image(reinterpret_cast<void*>(_Texture->GetID()), { width,height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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
				ImGui::Image(reinterpret_cast<void*>(_Texture->GetID()), { width,height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			}
		}
		else
		{
			ImGui::Text("No Sprite Selected");
		}
		/*
				ImVec2 pos = ImGui::GetWindowPos();
				ImVec2 size = ImGui::GetWindowSize();
		/ **/
		/*	ImVec4 color(1,1,1,1);
			ImU32 col = ImColor(color);
			ImGui::GetWindowDrawList()->AddLine({ pos.x + 100,pos.y }, { pos.x + 100,pos.y  +size.y }, col);* /*/
		ImGui::End();
	}
}
