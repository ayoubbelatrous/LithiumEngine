#include "epch.h"
#include "Panels/AnimationPanel.h"
#include "imgui/imgui.h"


namespace Lithium
{

	void AnimationPanel::OnCreate()
	{}

	void AnimationPanel::OnUpdate(bool* open)
	{
		ImGui::Begin("Animation",open);
		ImGui::End();
	}

}