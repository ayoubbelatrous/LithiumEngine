#include "InspectorPanel.h"
#include "imgui.h"
#include "entt.hpp"
#include "Scene/Components.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

namespace Lithium
{

	void InspectorPanel::OnCreate()
	{

	}

	void InspectorPanel::OnUpdate(Entity& _Selection)
	{

		
		ImGui::Begin("Inspector");
		
		if (_Selection.GetHandle() != entt::null)
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
			TransformComponent& trans = _Selection.GetComponent<TransformComponent>();
			ImGui::DragFloat3("Position",glm::value_ptr(trans.Position),0.01f);
			ImGui::DragFloat3("Rotation",glm::value_ptr(trans.Rotation),0.01f);
			ImGui::DragFloat3("Scale",glm::value_ptr(trans.Scale),0.01f);
		}
		if (_Selection.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& renderer = _Selection.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(renderer.Color));
		}
		}
		ImGui::End();
	}

}
