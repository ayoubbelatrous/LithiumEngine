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
		ImGui::Begin("Animation", open);
		DrawTimeLine();
		ImGui::End();
	}

	void AnimationPanel::DrawTimeLine()
	{
		float windowWidth = ImGui::GetWindowSize().x;
		float windowHeight = ImGui::GetWindowSize().y;
		ImVec2 windowPos = ImGui::GetWindowPos();

		float zoomPercentage = scroolBarMax - scroolBarMin;

		float posFromLeft = scroolBarMin * windowWidth;
		float TimeLineWidth = 500;


		if (windowWidth <= TimeLineWidth)
		{
			TimeLineWidth = windowWidth;
		}
		
		ImGui::SetCursorPos(ImVec2(-posFromLeft, 0));


		ImGui::Button("##timelinebutton", { (TimeLineWidth / zoomPercentage),60 });

		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImU32 lineColor = ImColor(ImVec4(1, 1, 1, 1));
		ImGui::Text("slider size %i", (int)(TimeLineWidth / zoomPercentage));
		ImGui::Text("posFromLeft %f", posFromLeft);
	
		for (size_t i = 0; i < (int)(TimeLineWidth/ zoomPercentage) / 50; i++)
		{
			drawlist->AddLine({ windowPos.x + (i * 50) + -posFromLeft,windowPos.y  + 50}, { windowPos.x + (i * 50) + -posFromLeft,windowPos.y + 59 }, lineColor);
		
			if (i % 5 == 0)
			{
				if (i > 9)
				{
					drawlist->AddText({ windowPos.x + (i * 50) + -posFromLeft + 5 ,windowPos.y + 25 }, lineColor, std::format("0:{}", i).c_str());
				}
				else
				{
					drawlist->AddText({ windowPos.x + (i * 50) + -posFromLeft + 5 ,windowPos.y + 25 }, lineColor, std::format("0:0{}", i).c_str());
				}
				drawlist->AddLine({ windowPos.x + (i * 50) + -posFromLeft,windowPos.y + 45 }, { windowPos.x + (i * 50) + -posFromLeft,windowPos.y + 59 }, lineColor);

			}
		}

		{
			
			ImGui::PushID(18);
			ImGui::SetCursorPos(ImVec2(5, windowHeight - 25));
			ImZoomSlider::ImZoomSlider(0.f, 1.0f, scroolBarMin, scroolBarMax, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_None);
			ImGui::PopID();
		}

	}

}