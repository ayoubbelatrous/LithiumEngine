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
		Ref<Texture> tex;
		Ref<Texture> tex2;


		float viewportSize[2];

		float orthosize = 5;
		ImVec2 _ViewportBounds[2];
		Entity _Selection;
		Ref<SceneHierachyPanel> _shp;
	};
}
