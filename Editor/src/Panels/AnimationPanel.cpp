#include "epch.h"
#include "Panels/AnimationPanel.h"
#include "Core/Log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "ImZoomSlider.h"
#include "Input/Input.h"
#include "Core/Application.h"
#include "Animation/Animation.h"

namespace Lithium
{

	void AnimationPanel::OnCreate()
	{
		m_KeyFrameIcon = CreateRef<Texture>("assets/Editor/icons/keyframe_128.png");
	}

	void AnimationPanel::OnUpdate(bool* open)
	{
		ImGui::Begin("Animation", open);
		DrawTimeLine();
		ImGui::End();
	}

	void AnimationPanel::DrawTimeLine()
	{

		Ref<Animation> animation;// Application::Get().assetManager->GetAsset<Ref<Animation>>(m_CurrentAnimation);
		if (animation == nullptr)
		{
			ImGui::Text("No Animation Set");
			return;
		}
		float windowWidth = ImGui::GetWindowSize().x;
		float windowHeight = ImGui::GetWindowSize().y;
		ImVec2 windowPos = ImGui::GetWindowPos();

		float zoomPercentage = m_ScrollBarMax - m_ScrollBarMin;

		float posFromLeft = m_ScrollBarMin * windowWidth;
		float TimeLineWidth = 500;


		if (windowWidth <= TimeLineWidth)
		{
			TimeLineWidth = windowWidth;
		}

		ImGui::SetCursorPos(ImVec2(-posFromLeft, 0));


		ImGui::Button("##timelinebutton", { (TimeLineWidth / zoomPercentage),60 });

		float dragDelta = ImGui::GetIO().MouseDelta.x;
		float ScaledDelta = dragDelta / windowWidth;

		if (Input::IsMouseKeyPressed(0) && ImGui::IsItemHovered())
		{

			m_ScrollBarMin -= ScaledDelta;
			m_ScrollBarMax -= ScaledDelta;

		}
		if (int mousewheel = ImGui::GetIO().MouseWheel)
		{
			m_ScrollBarMin -= mousewheel * 0.10;
			m_ScrollBarMax -= mousewheel * 0.10;
		}
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImU32 lineColor = ImColor(ImVec4(1, 1, 1, 1));

		float KeyTime = 0.01f;




		for (size_t i = 0; i < (int)(TimeLineWidth / zoomPercentage) / 50; i++)
		{
			drawlist->AddLine({ windowPos.x + (i * 50) + -posFromLeft,windowPos.y + 50 }, { windowPos.x + (i * 50) + -posFromLeft,windowPos.y + 59 }, lineColor);

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

		ImVec2 KeyPos = ImVec2(50, 20);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		for (size_t i = 0; i < animation->GetTracks().size(); i++)
		{

			if (animation->GetTracks()[i]->GetAnimationType() == AnimationType::TextureIndex)
			{
				TextureIndexTrack* pTrack = (TextureIndexTrack*)animation->GetTracks()[i];

				for (size_t i = 0; i < pTrack->GetKeyFrames().size(); i++)
				{

					KeyPos.x += pTrack->GetKeyFrames()[i].TimeStamp * (TimeLineWidth / zoomPercentage);
					ImGui::SetCursorPos(ImVec2(KeyPos.x - posFromLeft, KeyPos.y));

					if (ImGui::ImageButton((ImTextureID*)m_KeyFrameIcon->GetID(), { 25,25 }))
					{
						LT_CORE_INFO("keyframe pressed");
					}
				}
			}
		}
		ImGui::PopStyleColor(3);
		{
			ImGui::PushID(18);
			ImGui::SetCursorPos(ImVec2(5, windowHeight - 25));
			ImZoomSlider::ImZoomSlider(0.f, 1.0f, m_ScrollBarMin, m_ScrollBarMax, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_None);
			ImGui::PopID();
		}

	}

}