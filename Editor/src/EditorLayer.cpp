#include "lipch.h"
#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"
#include "Core/Math.h"
#include "Font/Font.h"
#include "Script/MonoServer.h"



namespace Lithium
{
	extern const std::filesystem::path root;

	void EditorLayer::OnCreate()
	{
		Application::GetInstance().GetImguiLayer()->SetBlockEvent(true);
		_monoserver = CreateRef<MonoServer>();

		_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		_EditorStatus = "";
		LastMousePosiition = glm::vec2(0);
		_shp = CreateRef<SceneHierachyPanel>();
		_InspectorPanel = CreateRef<InspectorPanel>();
		_InspectorPanel->OnCreate();
		_AssetBrowerPanel = CreateRef<AssetBrowserPanel>();
		_AssetBrowerPanel->SetEventCallback(BIND_EVENT(EditorLayer::onEditorEvent));

		
		_MainScene = CreateRef<Scene>();
		_MainScene->SetEventCallback(BIND_EVENT(EditorLayer::SceneEvent));
		Application::GetInstance()._sceneManager->SetActiveScene(_MainScene);

		_Selection = Entity(entt::null, _MainScene.get());
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

	
		Entity entity = _MainScene->CreateEntity("name");
		Entity entity3 = _MainScene->CreateEntity("dod");
		Entity entity2 = _MainScene->CreateEntity("hello");

		//_Selection = entity;
		_shp->SetScene(_MainScene);
		_InspectorPanel->SetScene(_MainScene);

		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));
		entity.AddComponent<TransformComponent>();

		entity2.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));
		entity2.AddComponent<TransformComponent>();

		entity3.AddComponent<SpriteRendererComponent>(glm::vec4(1, 1, 1, 1));
		entity3.AddComponent<ScriptComponent>("Test");
		ScriptComponent& sco = entity3.GetComponent<ScriptComponent>();
		sco._Scriptclass = _monoserver->GetClass(sco._name);
		//Ref<ScriptClass> scc = sco._Scriptclass;

		_MainScene->CopyComponent<TransformComponent>(entity,entity3);
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

	}

	void EditorLayer::OnUpdate()
	{
		//TODO: reload all classes in scriptcomponent if assembly changes
		
#pragma region CalculateProjection
		float AspectRatio = (float)viewportSize[0] / (float)viewportSize[1];
		float orthoLeft = -orthosize * AspectRatio * 0.5f;
		float orthoRight = orthosize * AspectRatio * 0.5f;
		float orthoBottom = -orthosize * 0.5f;
		float orthoTop = orthosize * 0.5f;

		proj = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop);
		framebuffer->resize(viewportSize[0], viewportSize[1]);
		DisplayBuffer->resize(viewportSize[0], viewportSize[1]);


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
		framebuffer->Bind();
	
		RendererCommand::ClearColor(glm::vec4(0.2, 0.2, 0.5, 1.0));
		RendererCommand::Clear();
		framebuffer->ClearAttachment(1, -1);
		BatchRenderer::Begin(view, proj);

		switch (_sceneState)
		{
		case (SceneState::EDITOR):
			{
			_MainScene->onEditorUpdate();
			if (_monoserver->CheckForChange())
			{
				auto view = _MainScene->GetRegistry().view<ScriptComponent>();

				for (auto entity : view)
				{

					ScriptComponent scc = view.get<ScriptComponent>(entity);
					std::string name = scc._name;

					Entity ent((entt::entity)entity, _MainScene.get());
					ent.RemoveComponent<ScriptComponent>();
					ent.AddComponent<ScriptComponent>(name);
					ScriptComponent& newscc = ent.GetComponent<ScriptComponent>();
					newscc._Scriptclass = _monoserver->GetClass(name);
					newscc.created = true;
					newscc._Scriptobject = ScriptClass::CreateInstance(newscc._Scriptclass);
					ScriptClass::InitObjectRuntime(newscc._Scriptobject);
					for (auto t : scc._Scriptobject->GetFields())
					{
						newscc._Scriptobject->GetFields()[t.first]->SetValue<int>(t.second->GetValue<int>());
					}
				}
			}
			break;
			}
		case (SceneState::RUNTIME):
		{
			_MainScene->onUpdate();
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
		

		if (!ImGuizmo::IsOver() && Input::IsMouseKeyPressed(0) && _ViewportFocus)
		{
			int pixeldata = framebuffer->ReadPixel(1, mouseX, mouseY);

			if (pixeldata == -1)
			{
				Entity entity(entt::null, _MainScene.get());

				_Selection = entity;
			}
			else if (pixeldata < -1)
			{

			}
			else
			{
				Entity entity((entt::entity)pixeldata, _MainScene.get());

				_Selection = entity;
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
		_InspectorPanel->SetSelection(_Selection);
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
		bool control = Input::IsKeyPressed(Key::LeftControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift);

		if (e.keycode == KEYCODE_S)
		{
			if (control)
			{
				
			}
		}
		
		if (e.keycode == KEYCODE_R)
		{
			if (control)
			{
				Ref<Scene> scene = CreateRef<Scene>();
				Serializer sz = Serializer(scene);
				sz.DeserializeScene("assets/scenes/test.lis");
				_MainScene = scene;
				_Selection = Entity(entt::null, nullptr);
			}
		}
		

		if (e.keycode == KEYCODE_E)
		{
			_GizmoMode = ImGuizmo::OPERATION::ROTATE;
		}
		if (e.keycode == KEYCODE_R)
		{
			_GizmoMode = ImGuizmo::OPERATION::SCALE;
		}
		if (e.keycode == KEYCODE_W)
		{
			_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		}



		if (e.keycode == KEYCODE_E)
		{
			if (control)
			{
				
				{
					Entity e = _MainScene->CreateEntity("new Entity");
					e.AddComponent<TransformComponent>();
				}

			}
		}

		if (e.keycode == KEYCODE_D && _Selection)
		{
			if (control)
			{
				if (e.action == GLFW_PRESS)
				{
					_MainScene->DuplicateEntity(_Selection);
				}

			}
		}
	



	}

	void EditorLayer::onMouseWheelEvent(MouseWheelEvent& e)
	{
		orthosize -= e.GetOffsetY() * 0.5;
	}

	void EditorLayer::onEditorEvent(Event& e)
	{
		

	}

	void EditorLayer::RenderImgui()
	{


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
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
		_shp->OnUpdate();
		_InspectorPanel->OnUpdate();
		_AssetBrowerPanel->OnUpdate();
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2,2 });

		ImGui::Begin("Scene");
		_ViewportHovered = ImGui::IsWindowHovered();
		_ViewportFocus = ImGui::IsWindowFocused();
		if (_ViewportFocus)
		{
			Application::GetInstance().GetImguiLayer()->SetBlockEvent(false);
		}


		viewportSize[0] = ImGui::GetContentRegionAvail().x;
		viewportSize[1] = ImGui::GetContentRegionAvail().y;
		ImGui::Image(reinterpret_cast<void*>(DisplayBuffer->GetColorAttachmentID(0)), ImGui::GetContentRegionAvail(), ImVec2{0, 1 }, ImVec2{ 1, 0 });
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
			{

				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturepath = root / path;
				if (hoveringEntity && hoveredEntity.HasComponent<SpriteRendererComponent>())
				{
					
				}

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



		if (_Selection.GetHandle() != entt::null && _Selection.HasComponent<TransformComponent>())
		{
			glm::mat4 matri = _Selection.GetComponent<TransformComponent>().GetMatrix();
			ImGuizmo::SetGizmoSizeClipSpace(0.2f);
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(_ViewportBounds[0].x, _ViewportBounds[0].y, _ViewportBounds[1].x - _ViewportBounds[0].x, _ViewportBounds[1].y - _ViewportBounds[0].y);
			ImGuizmo::Manipulate(glm::value_ptr(_view), glm::value_ptr(_proj),
				(ImGuizmo::OPERATION)_GizmoMode, ImGuizmo::WORLD, glm::value_ptr(matri));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(matri, translation, rotation, scale);
				//_Selection.GetComponent<TransformComponent>().Position = matri[3];

				glm::vec3 deltaRotation = rotation - _Selection.GetComponent<TransformComponent>().Rotation;
				_Selection.GetComponent<TransformComponent>().Position = translation;
				_Selection.GetComponent<TransformComponent>().Rotation += deltaRotation;
				_Selection.GetComponent<TransformComponent>().Scale = scale;
				UsingGizmos = true;
			}
			else
			{
				UsingGizmos = false;
			}

		}

		window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowBgAlpha(0.10f);
		float padding = 10;
		ImVec2 pos = ImVec2(_ViewportBounds[0].x + padding, _ViewportBounds[0].y + padding);
		ImGui::SetNextWindowPos(pos );
		//ImGui::SetNextWindowSize({ 30,30 });

		ImGui::Begin("win",(bool*)true,window_flags);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
		if (_sceneState == SceneState::RUNTIME) {
			if (ImGui::ImageButton((ImTextureID*)_StopButtonTexture->GetID(), { 30, 35 }))
			{
				StopRuntime();
			}
		}
		else if(_sceneState == SceneState::EDITOR)
		{
			if (ImGui::ImageButton((ImTextureID*)_PlayButtonTexture->GetID(), { 30, 35 }))
			{
				StartRuntime();
			}
		}
	
		ImGui::PopStyleVar(2);
		ImGui::End();

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
		//CORE_LOG(Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::Button("update assembly"))
		{
			_monoserver->CheckForChange();
		}
		ImGui::Text(_EditorStatus.c_str());
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

	void EditorLayer::SceneEvent(Event& e)
	{
		CreateEntityEvent& ev = static_cast<CreateEntityEvent&>(e);
	}

	void EditorLayer::StartRuntime()
	{
		/*std::unordered_map<std::string, ScriptComponent> Scriptmap;

		{
		 auto view = _MainScene->GetRegistry().view<ScriptComponent>();
		 for (auto entity : view)
		 {
			 Entity ent(entity, _MainScene.get());
			 ScriptComponent scc = ent.GetComponent<ScriptComponent>();
			 Scriptmap.emplace(scc._name, scc);
		 }
		}*/
		_EditorScene = Scene::Copy(_MainScene);


		/*{
			auto view = _MainScene->GetRegistry().view<ScriptComponent>();
			for (auto entity : view)
			{
				Entity ent(entity, _MainScene.get());
				ScriptComponent& scc = ent.GetComponent<ScriptComponent>();
				scc.created = false;
				scc._Scriptclass = _monoserver->GetClass(scc._name);
				scc._Scriptobject = nullptr;
				std::unordered_map<std::string, Ref<ScriptClassField>> Fieldtmap;
				auto script = Scriptmap[scc._name];
				for (auto t : scc._Scriptclass->GetFields())
				{
					auto field =script._Scriptobject->GetFields()[t.first];
					Ref<ScriptClassField> newfield = CreateRef<ScriptClassField>(field->GetFieldHandle(),nullptr);
					newfield->SetValue(field->GetValue());
					newfield->SetClass(field->GetClass());

					Fieldtmap.emplace(t.first, newfield);
				}
				scc._Scriptclass->SetFields(Fieldtmap);
			}
		}*/

		_sceneState = SceneState::RUNTIME;
		Application::GetInstance()._sceneManager->SetActiveScene(_MainScene);
	}

	void EditorLayer::StopRuntime()
	{
		_MainScene = _EditorScene;
		_sceneState = SceneState::EDITOR;
		Application::GetInstance()._sceneManager->SetActiveScene(_MainScene);

	}

}
