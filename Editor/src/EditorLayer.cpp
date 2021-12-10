#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"
#include "Core/Math.h"

namespace Lithium
{

	extern const std::filesystem::path root;
	bool intersectPlane(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& l0, const glm::vec3& l, float& t)
	{
		float denom = glm::dot(n, l);
		if (denom > 1e-6) {
			glm::vec3 p0l0 = p0 - l0;
			t = glm::dot(p0l0, n) / denom;
			return (t >= 0);
		}

		return false;
	}
	extern AssetMananger assetManager = AssetMananger();
	void EditorLayer::OnCreate()
	{

		Application::GetInstance().GetImguiLayer()->SetBlockEvent(true);
		_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		_EditorStatus = "";
		LastMousePosiition = glm::vec2(0);
		_shp = CreateRef<SceneHierachyPanel>();
		_InspectorPanel = CreateRef<InspectorPanel>();
		_InspectorPanel->OnCreate();
		_AssetBrowerPanel = CreateRef<AssetBrowserPanel>();

		LT_PROFILE_FUNCTION("init");

		_MainScene = CreateRef<Scene>();
		_MainScene->SetEventCallback(BIND_EVENT(EditorLayer::SceneEvent));
		_Selection = Entity(entt::null, _MainScene.get());
		framebuffer = CreateRef<FrameBuffer>();
		framebuffer->Bind();
		framebuffer->resize(1000, 1000);
		//sz = Serializer(_MainScene);
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
		entity3.AddComponent<TransformComponent>();

		pos = glm::vec3(0);
		view = glm::translate(glm::mat4(1), glm::vec3(0));

		proj = glm::ortho(-2.0, 2.0, -2.0, 2.0);
		model = glm::translate(glm::mat4(1), pos);

		_AssetBrowerPanel->OnCreate();

		//entity3.GetComponent<SpriteRendererComponent>().tex = assetManager.GetByHandle<Ref<Texture>>(0);
		BatchRenderer::Init();
	}

	void EditorLayer::OnUpdate()
	{
		LT_PROFILE_FUNCTION("UPDATE");
#pragma region CalculateProjection

		float AspectRatio = (float)viewportSize[0] / (float)viewportSize[1];
		float orthoLeft = -orthosize * AspectRatio * 0.5f;
		float orthoRight = orthosize * AspectRatio * 0.5f;
		float orthoBottom = -orthosize * 0.5f;
		float orthoTop = orthosize * 0.5f;

		proj = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop);
		framebuffer->resize(viewportSize[0], viewportSize[1]);

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
		//CORE_LOG(mouse.x<<mouse.y);

		//CORE_LOG(ray.x << ray.y << ray.z);
		framebuffer->Bind();
		RendererCommand::ClearColor(glm::vec4(0.25, 0.25, 0.35, 0));
		RendererCommand::Clear();
		framebuffer->ClearAttachment(1, -1);
		BatchRenderer::Begin(view, proj);

		_MainScene->onEditorUpdate();
		BatchRenderer::End();


		auto [mx, my] = ImGui::GetMousePos();
		mx -= _ViewportBounds[0].x;
		my -= _ViewportBounds[0].y;
		glm::vec2 vs = _ViewportBounds[1] - _ViewportBounds[0];
	    my = vs.y - my;
		mouseX = (int)mx;
		mouseY = (int)my;

		/*glm::vec3 ndc = Math::GetNormalizedDeviceCoords({ mouseX, mouseY }, { viewportSize[0], viewportSize[1] });
		//CORE_LOG(ray.x <<" " << ray.y);
		glm::vec4 ClipCoords = glm::vec4(ndc.x, ndc.y, -1, 1);

		glm::mat4 invertedproj = glm::inverse(proj);
		glm::vec4 eyecoords = invertedproj * ClipCoords;
		glm::vec4 rayeye = glm::vec4(eyecoords.x, eyecoords.y, -1.0, 0.0);
		glm::vec3 raywor = glm::vec3();
		raywor = (glm::inverse(view) * rayeye);
		//raywor = glm::translate(glm::inverse(view), { rayeye.x,rayeye.y,rayeye.z});
		raywor = glm::normalize(raywor);
		//CORE_LOG(raywor.x << " " << raywor.y);*/
	
		_Selection = _shp->GetSelection();
		if (!ImGuizmo::IsOver() &&  Input::IsMouseKeyPressed(0) && _ViewportFocus)
		{
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)vs.x && mouseY < (int)vs.y)
			{
				hoveringEntity = true;
				pixelData = framebuffer->ReadPixel(1, mouseX, mouseY);
				Entity ent{(entt::entity)pixelData,_MainScene.get()};
				_Selection = ent;
				_shp->SetSelection(_Selection);

				hoveredEntity = ent;
			}
			else
			{
				hoveringEntity = false;
				hoveredEntity = Entity();
			}

		}
		
		framebuffer->UnBind();
		
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

		
	}

	void EditorLayer::onKeyEvent(KeyEvent& e)
	{
		bool control = Input::IsKeyPressed(Key::LeftControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift);

		if (e.keycode == KEYCODE_S)
		{
			if (control)
			{
				_EditorStatus = "Saving Scene...";
				Serializer ser(_MainScene);
				//sz.SerializeScene("assets/scenes/main.lis");
				ser.SerializeScene("assets/scenes/test.lis");
				_EditorStatus = "";
			}
		}
		/*
		if (e.keycode == KEYCODE_R)
		{
			if (control)
			{
				Ref<Scene> scene = CreateRef<Scene>();
				//sz = Serializer(scene);
				_EditorStatus = "Reloading Scene...";
				//	sz.DeserializeScene("assets/scenes/main.lis");
				Serializer ser(_MainScene);
				ser.DeserializeScene("assets/scenes/test.lis");
				_MainScene = scene;
				_Selection = Entity(entt::null, nullptr);
				_InspectorPanel->SetScene(_MainScene);
				_shp->SetScene(_MainScene);
				_InspectorPanel->SetSelection(_shp->GetSelection());
				_EditorStatus = "";
			}
		}
		*/

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
				Entity e = _MainScene->CreateEntity("new Entity");
				e.AddComponent<TransformComponent>();
			}
		}
	}

	void EditorLayer::onMouseWheelEvent(MouseWheelEvent& e)
	{
		orthosize -= e.GetOffsetY() * 0.5;
	}

	void EditorLayer::RenderImgui()
	{
		LT_PROFILE_FUNCTION("Render Imgui");


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
		_shp->OnUpdate();
		_InspectorPanel->OnUpdate();
		_AssetBrowerPanel->OnUpdate();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2,2 });
		ImGui::Begin("Scene");
		_ViewportFocus = ImGui::IsWindowFocused();
		if (_ViewportFocus)
		{
			Application::GetInstance().GetImguiLayer()->SetBlockEvent(false);
		}
		

		viewportSize[0] = ImGui::GetContentRegionAvail().x;
		viewportSize[1] = ImGui::GetContentRegionAvail().y;
		ImGui::Image(reinterpret_cast<void*>(framebuffer->GetColorAttachmentID(0)), ImGui::GetContentRegionAvail(), ImVec2{0, 1 }, ImVec2{ 1, 0 });
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
			{
				
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturepath = root / path;
				if (hoveringEntity && hoveredEntity.HasComponent<SpriteRendererComponent>())
				{
					hoveredEntity.GetComponent<SpriteRendererComponent>().tex = assetManager.LoadAsset<Ref<Texture>>(texturepath.string());
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

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Begin("Stats");
		//CORE_LOG(Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
		CORE_LOG("Created Entity");
	}

}
