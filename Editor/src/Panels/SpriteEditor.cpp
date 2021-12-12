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
		ImGui::Begin("Sprite Editor");
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.25,0.25,0.25,1 });
		if (ImGui::Button("Slice"))
		{
			ImGui::OpenPopup("slice_options_popup");
		}
		ImGui::SameLine();
		ImGui::Button("Reset");
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
			ImGui::Image(reinterpret_cast<void*>(_Texture->GetID()), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		else
		{
			ImGui::Text("No Sprite Selected");
		}
		ImGui::End();
	}

}