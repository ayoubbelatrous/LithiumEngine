#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"

namespace Lithium
{
	void EditorLayer::OnCreate()
	{
		LastMousePosiition = glm::vec2(0);
		_shp = CreateRef<SceneHierachyPanel>();
		_InspectorPanel = CreateRef<InspectorPanel>();
		_AssetBrowerPanel  = CreateRef<AssetBrowserPanel>();

		LT_PROFILE_FUNCTION("init");
		
		_Selection = Entity(entt::null,_MainScene.get());
		framebuffer = CreateRef<FrameBuffer>();
		framebuffer->Bind();
		framebuffer->resize(1000, 1000);
		_MainScene = CreateRef<Scene>();

		Entity entity = _MainScene->CreateEntity("name");
		Entity entity2 = _MainScene->CreateEntity("hello");
		
		_Selection = entity;
		_shp->SetScene(_MainScene);
		_InspectorPanel->SetScene(_MainScene);
		
		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));
		entity.AddComponent<TransformComponent>();

		entity2.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));
		entity2.AddComponent<TransformComponent>();
		ChildManagerComponent& childmanager = entity2.GetComponent<ChildManagerComponent>();
		childmanager.AddChild(entity);
		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		SpriteRendererComponent& sp = entity.GetComponent<SpriteRendererComponent>();
		tc.Position = glm::vec3(-3, 0, 0);

		pos = glm::vec3(0);
		view = glm::mat4(0);
		view = model = glm::translate(glm::mat4(1), glm::vec3(1));

		proj = glm::ortho(-2.0, 2.0, -2.0, 2.0);
		model = glm::translate(glm::mat4(1), pos);
	
		Renderer2D::Init();
		_AssetBrowerPanel->OnCreate();
		//sz.SerializeScene(_MainScene, "assets/scenes/main.lis");
	}

	void EditorLayer::OnUpdate()
	{
#pragma region CalculateProjection

		float AspectRatio = (float)viewportSize[0] / (float)viewportSize[1];
		float orthoLeft = -orthosize * AspectRatio * 0.5f;
		float orthoRight = orthosize * AspectRatio * 0.5f;
		float orthoBottom = -orthosize * 0.5f;
		float orthoTop = orthosize * 0.5f;

		proj = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop);

#pragma endregion
#pragma region CameraMovement
		if (_ViewportFocus)
		{
			float speed = 0.01f;
			if (Input::IsKeyPressed(Key::W))
			{
				view = glm::translate(view, { 0,1 * speed,0 });
			}

			if (Input::IsKeyPressed(Key::S))
			{
				view = glm::translate(view, { 0,-1 * speed,0 });
			}

			if (Input::IsKeyPressed(Key::A))
			{
				view = glm::translate(view, { -1 * speed,0,0 });
			}

			if (Input::IsKeyPressed(Key::D))
			{
				view = glm::translate(view, { 1 * speed,0,0 });
			}

		}
	
#pragma endregion 
		
		framebuffer->Bind();
		RendererCommand::ClearColor(glm::vec4(0.25,0.25,0.35,0));
		RendererCommand::Clear();
		Renderer2D::BeginScene(proj, view);

		model = glm::translate(glm::mat4(1), { -0.0, 0.0, 0.0 });		

		_MainScene->onEditorUpdate();
		
		Renderer2D::EndScene();
		framebuffer->UnBind();
		RenderImgui();

	}

	void EditorLayer::OnDestroy()
	{
	}

	void EditorLayer::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyEvent>(BIND_EVENT(EditorLayer::onKeyEvent));
	}

	void EditorLayer::onKeyEvent(KeyEvent& e)
	{
		bool control = Input::IsKeyPressed(Key::LeftControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift);

		if (e.keycode == KEYCODE_S)
		{
			if (control)
			{
				sz.SerializeScene(_MainScene, "assets/scenes/main.lis");
			}
		}
	}

	void EditorLayer::RenderImgui()
	{


		if (!_ViewportFocus)
		{
			Application::GetInstance().GetImguiLayer()->SetBlockEvent(true);
		}
	
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}
		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		ImGui::End();
		_shp->OnUpdate(_Selection);
		_InspectorPanel->OnUpdate(_Selection);
		_AssetBrowerPanel->OnUpdate();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2,2 });
		ImGui::Begin("Scene");
		_ViewportFocus = ImGui::IsWindowFocused();
	
		viewportSize[0] = ImGui::GetContentRegionAvail().x;
		viewportSize[1] = ImGui::GetContentRegionAvail().y;
		ImGui::Image((void*)(intptr_t)framebuffer->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	
		ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		ImVec2 viewportOffset = ImGui::GetWindowPos();
		_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		glm::mat4 _view = view;
		glm::mat4 _proj = proj;
		
		
		if(_Selection.GetHandle() != entt::null && _Selection.HasComponent<TransformComponent>())
		{
			glm::mat4 matri = _Selection.GetComponent<TransformComponent>().GetMatrix();
			ImGuizmo::SetGizmoSizeClipSpace(0.2f);
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();
			
			ImGuizmo::SetRect(_ViewportBounds[0].x, _ViewportBounds[0].y, _ViewportBounds[1].x - _ViewportBounds[0].x, _ViewportBounds[1].y - _ViewportBounds[0].y);
			ImGuizmo::Manipulate(glm::value_ptr(_view), glm::value_ptr(_proj),
				(ImGuizmo::OPERATION)ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(matri));

			if (ImGuizmo::IsUsing())
			{
				_Selection.GetComponent<TransformComponent>().Position = matri[3];
			}
			
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Begin("Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
