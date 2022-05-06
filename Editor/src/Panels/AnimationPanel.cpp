#include "epch.h"
#include "Panels/AnimationPanel.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "ImZoomSlider.h"

namespace Lithium
{

	void AnimationPanel::OnCreate()
	{}

	void AnimationPanel::OnUpdate(bool* open)
	{
		ImGui::Begin("Animation",open);
		DrawTimeLine();
		ImGui::End();
	}

	void AnimationPanel::DrawTimeLine()
	{
		float windowWidth = ImGui::GetWindowSize().x;
		static float vMin = 0.4f, vMax = 0.6f;

		float zoomPercentage = vMax - vMin;

		float posFromLeft = vMin;
		float TimeLineWidth = 500;
		if (windowWidth <= TimeLineWidth)
		{
			TimeLineWidth = windowWidth;
		}

		ImGui::SetCursorPos(ImVec2(0, 0));

		
		ImGui::Button("##timelinebutton", { (TimeLineWidth / zoomPercentage),60 });

		{
			ImGui::PushID(18);
			ImZoomSlider::ImZoomSlider(0.f, 1.0f, vMin, vMax, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_None);
			ImGui::PopID();
		}

		ImGui::Text("slider size %f", (TimeLineWidth / zoomPercentage));
		ImGui::Text("posFromLeft %f", posFromLeft);

	}

}