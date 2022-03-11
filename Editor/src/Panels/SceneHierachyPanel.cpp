#include "lipch.h"

#include "SceneHierachyPanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "entt.hpp"
#include "Scene/Components.h"

namespace Lithium
{

	void SceneHierachyPanel::OnCreate()
	{
		
	}

	void SceneHierachyPanel::OnUpdate()
	{
		ImGui::Begin("Hierarchy");
		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));

		bool opened = ImGui::TreeNodeEx((void*)010101101, flags, "Scene");
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				const UUID* uuid = (UUID*)payload->Data;
				Entity DroppedEntity(_Scene->GetUUIDMap()[*uuid], _Scene.get());
				DroppedEntity.GetComponent<RelationShipComponent>().Parent = 0;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopStyleVar(2);
		if (opened)
		{
			_Scene->GetRegistry().each([&](auto entityID)
			{
					Entity entity{ entityID , _Scene.get() };
					if (entity.GetComponent<RelationShipComponent>().Parent == 0)
					{
						DrawEntity(entity);
					}
			});
			
		}
		ImGui::End();
	}
	
	 void SceneHierachyPanel::DrawEntity(Entity entity) {
		
		
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
		auto& tag = entity.GetComponent<NameComponent>().Name;
		ImGuiTreeNodeFlags flags = ((_Selection.GetHandle() == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));
		
		bool opened = ImGui::TreeNodeEx((void*)entity.GetHandle(), flags, tag.c_str());
		if (ImGui::BeginDragDropSource())
		{
			UUID id = entity.GetComponent<IDComponent>().ID;
			ImGui::SetDragDropPayload("ENTITY", &id, sizeof(UUID));

			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				const UUID* uuid = (UUID*)payload->Data;
				Entity DroppedEntity(_Scene->GetUUIDMap()[*uuid], _Scene.get());
				DroppedEntity.GetComponent<RelationShipComponent>().Parent = entity.GetComponent<IDComponent>().ID;
				entity.GetComponent<RelationShipComponent>().AddChild(*uuid);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			_Selection = entity;
		}
	
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}


		if (entityDeleted)
		{
			_Scene->DeleteEntity(entity);
			_Selection = Entity();
		}

		

		RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();


		if (opened)
		{
// 			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
// 			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
// 			if (opened)
// 				ImGui::TreePop();

			for (auto child : rc.Children)
			{
				Entity childEntity(_Scene->GetUUIDMap()[child], _Scene.get());
				
				{	
					DrawEntity(childEntity);
				}
			
			}
			ImGui::TreePop();
		}

		ImGui::PopStyleColor();
	 }
}