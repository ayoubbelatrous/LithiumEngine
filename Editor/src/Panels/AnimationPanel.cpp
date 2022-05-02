#include "epch.h"
#include "Panels/AnimationPanel.h"
#include "imgui/imgui.h"



namespace Lithium
{

	void AnimationPanel::OnCreate()
	{}

	void AnimationPanel::OnUpdate(bool* open)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::Begin("Animation", open);
		const ImU32 col = ImColor({ 0.0f,0.0f,0.0f,0.8f });
		const ImU32 textcol = ImColor({ 0.8f,0.8f,0.8f,0.8f });
		const ImU32 cursurcol = ImColor({ 0.8f,0.0f,0.0f,0.8f });
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 cpos = ImGui::GetCursorPos();
		
		ImGui::Button("##timeline", { ImGui::GetWindowSize().x,30 });
		float timeperpixel = 0.001f;
		float CursorTime = 0.0f;
		float CursorPos = 1000;
		{
			ImVec2 newpos = { winpos.x + cpos.x + CursorPos,winpos.y + cpos.y };
			ImVec2 lastpos = { winpos.x + cpos.x + CursorPos,winpos.y + cpos.y + ImGui::GetWindowSize().y - 3 };
			ImGui::GetWindowDrawList()->AddLine(newpos, lastpos, cursurcol, 1.5f);
		}
		
		for (size_t i = 0; i < ImGui::GetWindowSize().x / TimeLineScale; i++)
		{
			ImVec2 newpos = { 50 + winpos.x + cpos.x + TimeLineScale * i,winpos.y + cpos.y };
			ImVec2 lastpos = { 50 + winpos.x + cpos.x + TimeLineScale * i,winpos.y + cpos.y + 29 };

			ImVec2 textpos = { winpos.x + cpos.x + TimeLineScale * i + 5,winpos.y + cpos.y + 5 };

			ImGui::GetWindowDrawList()->AddLine(newpos, lastpos, col, 1.5f);
			if (i % 5 == 0)
			{
				ImGui::GetWindowDrawList()->AddText(textpos, textcol, std::format("{:.2f}", (float)(TimeLineScale * i) * timeperpixel).c_str());
			}
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::DragInt("Interval", &TimeLineScale);
		ImGui::End();
	
	}

}