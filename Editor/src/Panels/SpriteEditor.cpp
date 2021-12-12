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
		ImGui::Begin("Sprite Editor");
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