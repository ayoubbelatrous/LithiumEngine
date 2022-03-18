#include "lipch.h"
#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"
#include "Core/Math.h"
#include "Font/Font.h"
#include "Script/MonoServer.h"
#include <atomic>
#include "Panels/ProjectWizard.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
static std::atomic_bool canCheckAssembly;

namespace Lithium
{
	extern const std::filesystem::path root;

	void EditorLayer::OnCreate()
	{
		Application::Get().GetImguiLayer()->SetBlockEvent(true);
		_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		_EditorStatus = "";
		LastMousePosiition = glm::vec2(0);
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;
		_AssetBrowerPanel = CreateRef<AssetBrowserPanel>();
		m_SceneHierachyPanel = CreateRef<SceneHierachyPanel>();
		m_SceneHierachyPanel->OnCreate();


		m_InspectorPanel = CreateRef<InspectorPanel>();
		m_InspectorPanel->OnCreate();


		_AssetBrowerPanel->SetEventCallback(BIND_EVENT(EditorLayer::onEditorEvent));
		timer = CreateRef<Timer>();
		m_ActiveScene->SetEventCallback(BIND_EVENT(EditorLayer::SceneEvent));
		Application::Get().sceneManager->SetActiveScene(m_ActiveScene);

		m_SceneHierachyPanel->SetScene(m_ActiveScene);
		FrameBufferAttachmentDescriptor mainframebufferspec(
			{
				FramebufferTextureFormat::RGBA8,
			    FramebufferTextureFormat::RED_INTEGER,
			}
		);

		FrameBufferAttachmentDescriptor displayframebufferspec(
			{
				FramebufferTextureFormat::RGBA8,
			}
		);

		DisplayBuffer = CreateRef<FrameBuffer>(displayframebufferspec);
		DisplayBuffer->Bind();
		DisplayBuffer->resize(1000, 1000);
		framebuffer = CreateRef<FrameBuffer>(mainframebufferspec);
		framebuffer->Bind();
		framebuffer->resize(1000, 1000);

	
		Entity entity = m_ActiveScene->CreateEntity("Player");
		Entity entity3 = m_ActiveScene->CreateEntity("Camera");
		Entity entity2 = m_ActiveScene->CreateEntity("Pos");

		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));

		entity2.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));

		entity3.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));

		pos = glm::vec3(0);
		view = glm::translate(glm::mat4(1), glm::vec3(0));

		proj = glm::ortho(-2.0, 2.0, -2.0, 2.0);
		model = glm::translate(glm::mat4(1), pos);
		_AssetBrowerPanel->OnCreate();
		BatchRenderer::Init();
		shader = CreateRef<Shader>("assets/shaders/test.shader");
		frameshader = CreateRef<Shader>("assets/shaders/frame.shader");

		float rectangleVertices[] =
		{
			// Coords    // texCoords
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,

			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};

		vertarray = CreateRef<VertexArray>();
		vertbuffer = CreateRef<VertexBuffer>((void*)&rectangleVertices,sizeof(rectangleVertices));
		Ref<VertexBufferLayout> layout = CreateRef<VertexBufferLayout>();
		layout->Push<float>(2);
		layout->Push<float>(2);
		vertarray->AddBuffer(vertbuffer, layout);

		_PlayButtonTexture = CreateRef<Texture>("assets/Editor/icons/playbutton.png");
		_StopButtonTexture = CreateRef<Texture>("assets/Editor/icons/stopbutton.png");
		timer->SetInterval([=]()
		{
				canCheckAssembly.store(true);
		}, std::chrono::milliseconds(100));
		
	}

	void EditorLayer::OnUpdate()
	{
		LT_PROFILE_FUNCTION("OnUpdate");
		//TODO: reload all classes in scriptcomponent if assembly changes
		
#pragma region CalculateProjection
		float AspectRatio = (float)viewportSize[0] / (float)viewportSize[1];
		float orthoLeft = -orthosize * AspectRatio * 0.5f;
		float orthoRight = orthosize * AspectRatio * 0.5f;
		float orthoBottom = -orthosize * 0.5f;
		float orthoTop = orthosize * 0.5f;

		proj = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop);


		glm::quat orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	

		const glm::vec2& mouse{ Input::MousePosition() };
		m_CameraDelta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (Input::IsMouseKeyPressed(2))
		{

			float xSpeed = orthosize * 0.5f;
			float ySpeed = orthosize * 0.5f;

			m_FocalPoint += -glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f)) * m_CameraDelta.x * xSpeed;
			m_FocalPoint += glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f)) * m_CameraDelta.y * ySpeed;

		}

		m_CameraPosition = m_FocalPoint;// -glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 0.0f));
		view = glm::translate(glm::mat4(1.0f), m_CameraPosition) * glm::toMat4(orientation);
		view = glm::inverse(view);



		framebuffer->resize(viewportSize[0], viewportSize[1]);
		DisplayBuffer->resize(viewportSize[0], viewportSize[1]);


		

#pragma endregion
		if (m_ViewportFocus)
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
		framebuffer->Bind();
	
		RendererCommand::ClearColor(glm::vec4(0.05, 0.05, 0.15, 1.0));
		RendererCommand::Clear();
		framebuffer->ClearAttachment(1, -1);
		BatchRenderer::Begin(view, proj);

		switch (_sceneState)
		{
		case (SceneState::EDITOR):
			{
			m_ActiveScene->onEditorUpdate();
			if (canCheckAssembly)
			{
				if (Application::Get().Monoserver->CheckForChange())
				{
					ReloadMonoServer();
				}		
				canCheckAssembly.store(false);
			}
			
			break;
			}
		case (SceneState::RUNTIME):
		{
			m_ActiveScene->onUpdate();
			break;

		}
		}
		
		BatchRenderer::End();
		shader->Bind();
		shader->SetUniformMat4f("projection", model* proj * view );
	

		auto [mx, my] = ImGui::GetMousePos();
		mx -= _ViewportBounds[0].x;
		my -= _ViewportBounds[0].y;
		glm::vec2 vs = _ViewportBounds[1] - _ViewportBounds[0];
	    my = vs.y - my;
		mouseX = (int)mx;
		mouseY = (int)my;

		
		if (Input::IsMouseKeyPressed(0) && m_ViewportHovered && !ImGuizmo::IsOver())
		{
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)vs.x && mouseY < (int)vs.y)
			{
				pixelData = framebuffer->ReadPixel(1, mouseX, mouseY);
				if (pixelData == -1)
				{
					Entity ent{ entt::null,m_ActiveScene.get() };
					m_SceneHierachyPanel->SetSelection(ent);
				}
				else
				{
					Entity ent{(entt::entity)pixelData,m_ActiveScene.get() };
					m_SceneHierachyPanel->SetSelection(ent);
				}
			
			}
			else
			{
			}

		}

		framebuffer->UnBind();
		

		DisplayBuffer->Bind();
		
		RendererCommand::ClearColor(glm::vec4(0.00, 0.00, 0.00, 0));
		RendererCommand::Clear();
		vertarray->Bind();
		frameshader->Bind();
		framebuffer->BindTexture(0,0);
		frameshader->SetUniform1i("u_tex", framebuffer->GetColorAttachmentID());
		RendererCommand::Draw(6);

		DisplayBuffer->UnBind();

		RenderImgui();
	}

	void EditorLayer::OnDestroy()
	{
	}


	void EditorLayer::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyEvent>(BIND_EVENT(EditorLayer::onKeyEvent));
		dispatcher.Dispatch<MouseWheelEvent>(BIND_EVENT(EditorLayer::onMouseWheelEvent));
		if (e.GetEventType() == EventType::WindowFileDrop)
		{
			WindowFileDropEvent& dropevent = static_cast<WindowFileDropEvent&>(e);
			std::filesystem::copy(dropevent.getPaths()[0],_AssetBrowerPanel->GetCurrentPath());
			_AssetBrowerPanel->Refresh();

		}

	}

	void EditorLayer::onKeyEvent(KeyEvent& e)
	{
		if (e.action == GLFW_REPEAT)
		{
			return;
		}
		bool control = Input::IsKeyPressed(Key::LeftControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift);

		if (e.keycode == KEYCODE_S)
		{
			if (control && shift)
			{
				m_OpenSceneSaveDialog = true;
			}
		}

		if (m_ViewportFocus)
		{
			if (e.keycode == KEYCODE_E)
			{
				_GizmoMode = ImGuizmo::OPERATION::ROTATE;
				m_UseBoundsGizmo = false;

			}
			if (e.keycode == KEYCODE_R)
			{
				_GizmoMode = ImGuizmo::OPERATION::SCALE;
				m_UseBoundsGizmo = false;

			}
			if (e.keycode == KEYCODE_W)
			{
				_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
				m_UseBoundsGizmo = false;

			}

			if (e.keycode == KEYCODE_T)
			{
				_GizmoMode = ImGuizmo::OPERATION::BOUNDS;
				m_UseBoundsGizmo = true;
			}

		}

		if (e.keycode == KEYCODE_E)
		{
			if (control)
			{
				
				{
					Entity e = m_ActiveScene->CreateEntity("new Entity");
					e.AddComponent<TransformComponent>();
				}

			}
		}

		if (e.keycode == KEYCODE_D && m_SceneHierachyPanel->GetSelection())
		{
			if (control)
			{
				if (e.action == GLFW_PRESS)
				{
					m_ActiveScene->DuplicateEntity(m_SceneHierachyPanel->GetSelection());
				}

			}
		}
	}

	void EditorLayer::onMouseWheelEvent(MouseWheelEvent& e)
	{
		
		float distance = orthosize * 0.5f * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 5.0f);
		orthosize -= e.GetOffsetY() * speed;
	}

	void EditorLayer::onEditorEvent(Event& e)
	{
		

	}

	void EditorLayer::RenderImgui()
	{
		LT_PROFILE_FUNCTION("RenderImGui()");

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


	
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoDocking;
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
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		if (m_Project != nullptr)
		{
			ProjectWizard::Update();
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
			return;
		}
		if (ImGui::BeginMenuBar())
		{
			
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"));


				if (ImGui::MenuItem("Open...", "Ctrl+O"));
					

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"));
				ImGui::EndMenu();
			}
			if (_sceneState == SceneState::RUNTIME) {
				
				if (ImGui::ImageButton((ImTextureID*)_StopButtonTexture->GetID(), { 15,15}))
				{

					StopRuntime();
					m_SceneHierachyPanel->SetScene(m_ActiveScene);

				}
			}
			else if (_sceneState == SceneState::EDITOR)
			{
				if (ImGui::ImageButton((ImTextureID*)_PlayButtonTexture->GetID(), { 15,15 }))
				{
				
					StartRuntime();
					m_SceneHierachyPanel->SetScene(m_ActiveScene);

				}
			}
			ImGui::EndMenuBar();


		}
		{
			LT_PROFILE_SCOPE("SceneHierachyPanel()")
				m_SceneHierachyPanel->OnUpdate();
			LT_END_SCOPE();
		}
	
		{

			LT_PROFILE_SCOPE("inspector()")
				m_InspectorPanel->SetSelection(m_SceneHierachyPanel->GetSelection());
				m_InspectorPanel->OnUpdate();
			LT_END_SCOPE();
		}
		{
			LT_PROFILE_SCOPE("assetbrowser()")
			_AssetBrowerPanel->OnUpdate();
			LT_END_SCOPE();

		}
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5,5 });

		ImGui::Begin("Scene");
		
		m_ViewportHovered = ImGui::IsWindowHovered();
		m_ViewportFocus = ImGui::IsWindowFocused();

		if (m_ViewportFocus)
		{
			Application::Get().GetImguiLayer()->SetBlockEvent(false);
		}



	
		viewportSize[0] = ImGui::GetContentRegionAvail().x;
		viewportSize[1] = ImGui::GetContentRegionAvail().y;
		ImGui::Image(reinterpret_cast<void*>(DisplayBuffer->GetColorAttachmentID(0)), ImGui::GetContentRegionAvail(), ImVec2{0, 1 }, ImVec2{ 1, 0 });

		//Drag And Drop
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path ScenePath = root / path;
				OpenScene(ScenePath.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		ImVec2 viewportOffset = ImGui::GetWindowPos();
		_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		glm::mat4 _view = view;
		glm::mat4 _proj = proj;

		Entity selected = m_SceneHierachyPanel->GetSelection();


		//ImGuizmo::SetGizmoSizeClipSpace(0.2f);
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(_ViewportBounds[0].x, _ViewportBounds[0].y, _ViewportBounds[1].x - _ViewportBounds[0].x, _ViewportBounds[1].y - _ViewportBounds[0].y);


		if (selected.GetHandle() != entt::null && selected.HasComponent<TransformComponent>())
		{
			glm::mat4 matri = selected.GetComponent<TransformComponent>().GetMatrix();
			UUID parentUUID = selected.GetComponent<RelationShipComponent>().Parent;
			Entity ParentEntity(m_ActiveScene->GetUUIDMap()[parentUUID], m_ActiveScene.get());
			TransformComponent& ParentTransform = ParentEntity.GetComponent<TransformComponent>();
			float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
			ImGuizmo::Manipulate(glm::value_ptr(_view), glm::value_ptr(_proj), (ImGuizmo::OPERATION)_GizmoMode, ImGuizmo::WORLD
				, glm::value_ptr(matri),NULL,NULL,m_UseBoundsGizmo ? bounds : NULL);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(matri, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - selected.GetComponent<TransformComponent>().Rotation;
				selected.GetComponent<TransformComponent>().Position = translation;
				selected.GetComponent<TransformComponent>().Rotation = glm::degrees(rotation);
				selected.GetComponent<TransformComponent>().Scale = scale;
				UsingGizmos = true;
			}
			else
			{
				UsingGizmos = false;
			}

		}


		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::Begin("Console");
		if (ImGui::Button("Clear"))
		{
			MonoServer::_BufferLog.clear();
		}
		for (const char* log : MonoServer::_BufferLog)
		{
			ImGui::Selectable(log);
		}
		ImGui::End();
		ImGui::Begin("Stats");
		ImGui::DragFloat3("camera", glm::value_ptr(m_CameraPosition));
		ImGui::DragFloat3("focal point",glm::value_ptr(m_FocalPoint));
		float AspectRatio = (float)viewportSize[0] / (float)viewportSize[1];

		ImGui::DragFloat("aspect", &AspectRatio);
		ImGui::DragFloat("ortho size", &orthosize);
		ImGui::Text("mouse delta x(%.4f) , y(%.4f)", m_CameraDelta.x * orthosize * 0.5f, m_CameraDelta.y * orthosize * 0.5f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text(_EditorStatus.c_str());

	
		ImGui::End();

		if (m_OpenSceneSaveDialog)
			ImGui::OpenPopup("Scene Save");

		if (ImGui::BeginPopupModal("Scene Save"))
		{
			ImGui::Text("Save Scene As..");
			char buffer[256];
			memset(buffer, 0, 256);
			strcpy(buffer,m_ScenePath.c_str());
			if (ImGui::InputText("Path", buffer, 256))
			{
				m_ScenePath = buffer;
			}


			if (ImGui::Button("Save"))
			{
				CORE_LOG(buffer);
				Serializer serilizer(m_ActiveScene);
				serilizer.SerializeScene(m_ScenePath);
				m_OpenSceneSaveDialog = false;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				m_OpenSceneSaveDialog = false;
			}
			ImGui::EndPopup();
		}
		

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

	void EditorLayer::ImGuiRenderToolBar()
	{
	}

	void EditorLayer::SceneEvent(Event& e)
	{
		CreateEntityEvent& ev = static_cast<CreateEntityEvent&>(e);
	}

	void EditorLayer::StartRuntime()
	{
		
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnStart();
		m_SceneHierachyPanel->SetScene(m_ActiveScene);
		_sceneState = SceneState::RUNTIME;

		Application::Get().sceneManager->SetActiveScene(m_ActiveScene);
		Application::Get().Monoserver->ForceReload();


		auto view = m_ActiveScene->GetRegistry().view<ScriptGroupeComponent>();
		for (auto e : view)
		{
			Entity entity = { e,m_ActiveScene.get() };
			ScriptGroupeComponent& ScriptGroupe = entity.GetComponent<ScriptGroupeComponent>();
			for (auto& script : ScriptGroupe.Scripts)
			{
				
				Ref<ScriptObject> OldScriptobject = script.Scriptobject;
				if (script.Loaded)
				{
					script.Scriptobject = Application::Get().Monoserver->CopyObject(OldScriptobject);

				}

			}
		}
	}

	void EditorLayer::StopRuntime()
	{
		m_ActiveScene->OnStop();
		m_ActiveScene = m_EditorScene;
		_sceneState = SceneState::EDITOR;
		Application::Get().sceneManager->SetActiveScene(m_ActiveScene);
		m_SceneHierachyPanel->SetScene(m_ActiveScene);

		Application::Get().Monoserver->ForceReload();
		auto view = m_ActiveScene->GetRegistry().view<ScriptGroupeComponent>();
		for (auto e : view)
		{
			Entity entity = { e,m_ActiveScene.get() };
			ScriptGroupeComponent& ScriptGroupe = entity.GetComponent<ScriptGroupeComponent>();
			for (auto& script : ScriptGroupe.Scripts)
			{
				Ref<ScriptObject> OldScriptobject = script.Scriptobject;
				if (script.Loaded)
				{
					script.Scriptobject = Application::Get().Monoserver->CopyObject(OldScriptobject);
				}
			}
		}
	}

	void EditorLayer::ReloadMonoServer()
	{
		Application::Get().Monoserver->ForceReload();
		{
			auto view = m_ActiveScene->GetRegistry().view<ScriptGroupeComponent>();
			for (auto e : view)
			{
				Entity entity = { e,m_ActiveScene.get() };
				ScriptGroupeComponent& ScriptGroupe = entity.GetComponent<ScriptGroupeComponent>();
				for (auto& script:ScriptGroupe.Scripts)
				{
					Ref<ScriptObject> OldScriptobject = script.Scriptobject;
					script.Scriptobject = Application::Get().Monoserver->CopyObject(OldScriptobject);

				}
			}
		}
	}

	void EditorLayer::OpenScene(const std::string& path)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		Serializer sz = Serializer(scene);
		sz.DeserializeScene(path);
		m_EditorScene = scene;
		m_ActiveScene = m_EditorScene;
		m_SceneHierachyPanel->SetScene(m_ActiveScene);
		Application::Get().sceneManager->SetActiveScene(m_ActiveScene);
		ReloadMonoServer();
	}

}
