#include "InspectorPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "entt.hpp"
#include "Scene/Components.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Core/Base.h"
#include <filesystem>
#include "AssetManager/AssetManager.h"


namespace Lithium
{
	extern const std::filesystem::path root;
	extern AssetMananger assetManager;
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	void InspectorPanel::OnCreate()
	{
		assetManager.LoadAsset<Ref<Texture>>("assets/images/test.png");
	}

	void InspectorPanel::OnUpdate()
	{

		
		ImGui::Begin("Inspector");
		
		if (_Selection.GetHandle() != entt::null && _Selection.getScene() != nullptr)
		{
		
		if (_Selection.HasComponent<NameComponent>())
		{
			NameComponent& namec =  _Selection.GetComponent<NameComponent>();
				
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, namec.GetName().c_str(), sizeof(buffer));
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				namec.GetName() = std::string(buffer);
			}
			
		}

		if (_Selection.HasComponent<TransformComponent>())
		{
			
			ImGui::Selectable("Transform");

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Remove Component"))
				{
				}
				if (ImGui::MenuItem("Copy Component"))
				{
				}

				ImGui::EndPopup();
			}

			ImGui::Separator();
			DrawVec3Control("Position", _Selection.GetComponent<TransformComponent>().Position);
			ImGui::Separator();
			DrawVec3Control("Rotation", _Selection.GetComponent<TransformComponent>().Rotation);
			ImGui::Separator();
			DrawVec3Control("Scale", _Selection.GetComponent<TransformComponent>().Scale);
			ImGui::Separator();
		}
		if (_Selection.HasComponent<SpriteRendererComponent>())
		{
			ImGui::Selectable("Sprite Renderer");
			
		
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
					}
					if (ImGui::MenuItem("Copy Component"))
					{
					}

					ImGui::EndPopup();
				}

				
				
			ImGui::Separator();
			ImGui::ColorEdit4("Color", glm::value_ptr(_Selection.GetComponent<SpriteRendererComponent>().Color));
			if (_Selection.GetComponent<SpriteRendererComponent>().tex->loaded)
			{
				ImGui::Image((ImTextureID)_Selection.GetComponent<SpriteRendererComponent>().tex->GetID(), {75,75});
			}
			else
			{
				ImGui::Button("Texture", { ImGui::GetContentRegionAvail().x,50 });
			}
			
			
			if (ImGui::BeginDragDropTarget())
			{

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturepath = root/path;
					_Selection.GetComponent<SpriteRendererComponent>().tex = assetManager.LoadAsset<Ref<Texture>>(texturepath.string());
				}
				ImGui::EndDragDropTarget();
			}
		  }

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("add_componenet_popup");
		}


		if (ImGui::BeginPopup("add_componenet_popup"))
		{
			if (!_Selection.HasComponent<SpriteRendererComponent>())
			{

			  if (ImGui::MenuItem("Sprite Renderer"))
			  {
			
				_Selection.AddComponent<SpriteRendererComponent>();
			  }
			}


			if (!_Selection.HasComponent<TransformComponent>())
			{

				if (ImGui::MenuItem("Transform"))
				{

					_Selection.AddComponent<TransformComponent>();
				}
			}
			ImGui::EndPopup();

		}
		}
		
		ImGui::End();
	}

}

