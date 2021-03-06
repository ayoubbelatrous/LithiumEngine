#pragma once
#include "Lithium.h"
#include "Core/Base.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/transform.hpp"

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
#include "Scene/Serializer.h"
#include "AssetManager/AssetManager.h"
#include <thread>
#include "Mesh/Mesh.h"
#include "Renderer/UniformBuffer.h"
#include "panels/SceneHierachyPanel.h"

namespace Lithium
{
	struct CameraBuffer
	{
		glm::mat4 u_proj;
	};
	struct Data
	{
		std::string name = "entity";
	};
	class EditorLayer : public Lithium::Layer
	{
	public:
		EditorLayer()
			:Layer("editor")
		{
			//sz = Serializer(_MainScene);
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void onMouseWheelEvent(MouseWheelEvent& e);
		void RenderImgui();
		void BeginImgui();
		void EndImGui();
	private:
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;
		//Render passes
		Ref<FrameBuffer> _MainFramebuffer;
		//imgui ui
		glm::vec2 ViewportSize;
		bool openHierachy;
		Ref<Scene> _MainScene;
		Ref<UniformBuffer> _ub;
		std::vector<Ref<Mesh>> meshes;
		Ref<Shader> shader;
		Entity entity;
		Ref<Material> material;
		CameraBuffer data;
		Ref<Texture> diffuse;
		Ref<SceneHierachyPanel> _HierachyPanel;
		Handle<Ref<Texture>> texasset;
	};
}
