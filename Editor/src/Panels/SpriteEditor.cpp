#include "SpriteEditor.h"
#include "imgui.h"



namespace Lithium
{

	void SpriteEditor::OnCreate()
	{

	}

	void SpriteEditor::SetTexture(const Ref<Texture>& texture)
	{
		_Texture = texture;
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
		ImGui::Button("Reset");
		ImGui::SameLine();
		ImGui::SetCursorPosX(width - 10);
		if (ImGui::Button("X", { 25,25 }))
		{
			Close();
		}
		ImGui::PopStyleColor();
		if (ImGui::BeginPopup("slice_options_popup"))
		{
			float* test = new float[2];
			ImGui::InputFloat2("Cell Size", test);
			ImGui::Button("Slice");
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
		

			ImGui::Image(reinterpret_cast<void*>(_Texture->GetID()), {width,height}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			
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
