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
#include "Panels/SceneHierachyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/SpriteEditor.h"
#include "Scene/Serializer.h"
#include "AssetManager/AssetManager.h"
#include <thread>
#include "Mesh/Mesh.h"
#include "Script/MonoServer.h"
#include "Script/ScriptClass.h"
#include "Script/ScriptObject.h"
#include "Core/Timer.h"

namespace Lithium
{
	enum class SceneState
	{
		None = 0,
		PAUSE,
		EDITOR,
		RUNTIME
	};
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
			//sz = Serializer(_MainScene);
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void onMouseWheelEvent(MouseWheelEvent& e);
		void onEditorEvent(Event& e);
		void RenderImgui();
		void SceneEvent(Event& e);
		void StartRuntime();
		void StopRuntime();
	private:
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;

		Ref<FrameBuffer> framebuffer;
		Ref<FrameBuffer> DisplayBuffer;
		Ref<Scene> _MainScene;
		Ref<Scene> _RuntimeScene;
		Ref<Scene> _EditorScene;

		bool _ViewportHovered = false;
		bool _ViewportFocus = false;
		std::string _EditorStatus;

		float viewportSize[2];

		float orthosize = 5;
		glm::vec2 _ViewportBounds[2];
		Entity _Selection;
		Ref<SceneHierachyPanel> _shp;
		Ref<InspectorPanel> _InspectorPanel;
		Ref<AssetBrowserPanel> _AssetBrowerPanel;
		int mouseX = 0;
		int mouseY = 0;
		int pixelData; 
		float pixelData2;
		bool UsingGizmos;
		std::string text;
		glm::vec2 LastMousePosiition;
		int _GizmoMode = -1;
		bool hoveringEntity = false;
		Entity hoveredEntity;
		
		Ref<Shader> shader;
		Ref<Shader> frameshader;


		Ref<VertexArray> vertarray;
		Ref<VertexBuffer> vertbuffer;

		Ref<MonoServer> _monoserver;

		Ref<Texture> _PlayButtonTexture;
		Ref<Texture> _StopButtonTexture;
		Ref<Texture> _PauseTexture;
		SceneState _sceneState = SceneState::EDITOR;
		Serializer _SceneSerilizer = Serializer(_MainScene);

		Ref<Timer> timer;
	};
}
