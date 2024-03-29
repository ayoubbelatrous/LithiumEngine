#pragma once
#include "Lithium.h"
#include "Core/Base.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

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
#include "imgui/imgui.h"
#include <../imgui/example/imgui_impl_glfw.h>
#include <../imgui/example/imgui_impl_opengl3.h>
#include "Panels/SceneHierachyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/AnimationPanel.h"
#include "Panels/AssetPropertiesPanel.h"
#include "Panels/SpriteEditor.h"
#include "Scene/Serializer.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/AssetPacker.h"
#include <thread>
#include "Script/MonoServer.h"
#include "Core/Timer.h"
#include "Scene/Camera/EditorCamera.h"
#include "Audio/Audio.h"
#include "Audio/AudioSource.h"
#include "Font/Font.h"
#include "Utils/WindowsPlatformUtils.h"
#include "Animation/Animation.h"
#include "SceneRenderer/Mesh/Mesh.h"
#include "SceneRenderer/Mesh/MeshLoader.h"

#include "Core/RecentProjectManager.h"

namespace Lithium
{
	enum class SceneState
	{
		None = 0,
		PAUSE,
		EDITOR,
		RUNTIME
	};

	struct Project
	{
		std::filesystem::path CurrentProjectPath;
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
		void onMouseKeyPressEvent(MouseKeyPressEvent& e);
		void onEditorEvent(Event& e);
		void OnImguiRender() override;
		void DebugRender();
		void ImGuiRenderToolBar();
		void SceneEvent(Event& e);
		void StartRuntime();
		void StopRuntime();
		void ReloadMonoServer();
		void OpenScene(const std::string& path);
		void OpenProjectDialog();
		void OpenProject(std::string path);
	private:
		glm::mat4 proj;
		glm::mat4 UIProjection;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;

		Ref<FrameBuffer> framebuffer;
		Ref<FrameBuffer> DisplayBuffer;
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		bool m_ViewportHovered = false;
		bool m_ViewportFocus = false;
		bool m_ActiveSceneSaved = false;
		std::string _EditorStatus;

		float viewportSize[2];
		bool m_ViewportSizeChanged = true;
		float orthosize = 5;
		glm::vec2 _ViewportBounds[2];
		Ref<AssetBrowserPanel> _AssetBrowerPanel;
		int mouseX = 0;
		int mouseY = 0;
		int pixelData;
		float pixelData2;
		bool UsingGizmos;
		std::string text;
		glm::vec2 LastMousePosiition;
		int _GizmoMode = -1;
		bool m_UseBoundsGizmo = false;
		bool m_CanPick = false;
		Ref<Shader> shader;
		Ref<Shader> frameshader;


		Ref<VertexArray> vertarray;
		Ref<VertexBuffer> vertbuffer;

		Ref<Texture> _PlayButtonTexture;
		Ref<Texture> _StopButtonTexture;
		Ref<Texture> _PauseTexture;
		Ref<Texture> m_CameraGizmo;
		SceneState _sceneState = SceneState::EDITOR;
		Serializer m_SceneSerilizer = Serializer(m_ActiveScene);

		Ref<Timer> timer;


		//Panels 
		Ref<SceneHierachyPanel> m_SceneHierachyPanel;
		Ref<AnimationPanel> m_AnimationPanel;
		Ref<InspectorPanel> m_InspectorPanel;
		Ref<AssetPropertiesPanel> m_AssetPropertiesPanel;
		Ref<SpriteEditor> m_SpriteEditor;
		std::string m_ScenePath = "assets/scenes/";
		bool m_OpenStatsPanel = true;
		bool m_OpenProjectWizard = false;
		bool m_OpenAnimationPanel = true;
		bool m_OpenAssetPropertiesPanel = false;
		bool m_OpenSpriteEditor = false;
		glm::vec2 m_InitialMousePosition = { 0.0f,0.0f };
		glm::vec2 m_CameraDelta = { 0.0f,0.0f };
		glm::vec3 m_FocalPoint = glm::vec3(0);
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		AudioSource* source;
		bool m_StoppedUsingGizmos;
		Ref<Rendering::Loaders::MeshLoader> m_MeshLoader;

		Ref<Core::RecentProjectManager> m_RecentProjectManager;
	};
}
