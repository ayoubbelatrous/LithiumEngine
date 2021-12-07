#pragma once
#include "Lithium.h"
#include "Core/Base.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FrameBuffer.h"
#include "Event/EventDispatcher.h"
#include "Event/AppEvents.h"
#include "Scene/Scene.h"
#include "entt.hpp"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "imgui.h"
#include <../imgui/example/imgui_impl_glfw.h>
#include <../imgui/example/imgui_impl_opengl3.h>
#include "Panels/SceneHierachyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Scene/Serializer.h"
#include <array>
namespace Lithium
{
	struct Data
	{
		std::string name = "entity";
	};
	class EditorLayer : public Lithium::Layer
	{
	public:
		EditorLayer()
			:Layer("hi")
		{
			sz = Serializer(_MainScene);
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void RenderImgui();
	private:
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;

		Ref<FrameBuffer> framebuffer;
		Ref<Scene> _MainScene;

		bool _ViewportHovered = false;
		bool _ViewportFocus = false;
		std::string _EditorStatus;

		float viewportSize[2];

		float orthosize = 5;
		ImVec2 _ViewportBounds[2];
		Entity _Selection;
		Ref<SceneHierachyPanel> _shp;
		Ref<InspectorPanel> _InspectorPanel;
		Ref<AssetBrowserPanel> _AssetBrowerPanel;
		Serializer sz;
		Ref<Texture> tex;
		Ref<Texture> tex2;
		glm::vec2 LastMousePosiition;
	};
}
