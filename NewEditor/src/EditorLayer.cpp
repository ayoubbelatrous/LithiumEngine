#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"
#include "Core/Math.h"
#include "Font/Font.h"
#include "Mesh/Material.h"



namespace Lithium
{
	
	void EditorLayer::OnCreate()
	{

		FrameBufferAttachmentDescriptor mainframebufferdescryptor(
			{
				FramebufferTextureFormat::RGBA8,
				FramebufferTextureFormat::Depth,
			}
		);
		_MainFramebuffer = CreateRef<FrameBuffer>(mainframebufferdescryptor);
		_HierachyPanel = CreateRef<SceneHierachyPanel>();
		_MainScene = CreateRef<Scene>();

	
		shader = CreateRef<Shader>("assets/shaders/test.shader");
		material = Material::MaterialFromShader(shader);
		Material::MaterialToFile(material, "assets/mat.mat");
		material = Material::MaterialFromFile("assets/mat.mat");

		proj = glm::perspective(glm::radians(45.0f), (float)ViewportSize.x / (float)ViewportSize.y, 0.1f, 100.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-10));
		view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f,-6.0));

		_ub = CreateRef<UniformBuffer>(sizeof(CameraBuffer),0);
		data.u_proj = proj * view;

		_ub->SetData(&data, sizeof(data), 0);
		
		
		meshes = Mesh::LoadMesh("assets/model/test.obj");
		diffuse= CreateRef<Texture>("assets/test.png");
		material->SetShader(shader);

		entity = _MainScene->CreateEntity("entity");
		entity.AddComponent<MaterialComponent>(material);
		entity.AddComponent<MeshRendererComponent>();
		entity.AddComponent<MeshComponent>();
		entity.AddComponent<TransformComponent>();




		entity.GetComponent<MeshComponent>()._Mesh = meshes[0];
		_HierachyPanel->SetScene(_MainScene);
		_HierachyPanel->OnCreate();

		
		texasset = Application::GetInstance().assetManager->LoadAsset<Ref<Texture>>("assets/images/test.jpg");
		//t = Application::GetInstance().assetManager->LoadAsset<Ref<Texture>>("assets/images/test.jpg");
		//Handle<Ref<Texture>> b = Application::GetInstance().assetManager->LoadAsset<Ref<Texture>>("assets/images/test.jpg");

	}
	void EditorLayer::OnUpdate()
	{
		proj = glm::perspective(glm::radians(45.0f), (float)ViewportSize.x / (float)ViewportSize.y, 0.1f, 100.0f);

		data.u_proj = proj * view;
		model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		_ub->SetData(&data, sizeof(data), 0);

		_MainFramebuffer->resize(ViewportSize.x, ViewportSize.y);
		_MainFramebuffer->Bind();
		entity.GetComponent<TransformComponent>().Rotation = glm::vec3(glm::radians(60.0f),0,0);
		RendererCommand::ClearColor(glm::vec4(0.25,0.11,0.54,1.0));
		RendererCommand::Clear();
		_MainScene->onEditorUpdate();
		_MainFramebuffer->UnBind();
		
		RenderImgui();
		
	}

	void EditorLayer::RenderImgui()
	{
		BeginImgui();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("scene");
		ViewportSize.x = ImGui::GetContentRegionAvail().x;
		ViewportSize.y = ImGui::GetContentRegionAvail().y;
		ImGui::Image(reinterpret_cast<ImTextureID>(_MainFramebuffer->GetColorAttachmentID(0)),ImVec2(ViewportSize.x,ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::Begin("Inspector", &openHierachy);
		ImGui::Text("shader %s", shader->GetPath().c_str());
		ImGui::Separator();
		for (auto field : material->GetDataFields())
		{
			if (field->getType() == MaterialDataType::Vec4)
			{
				Vec4* type = (Vec4*)field.get();
				ImGui::ColorEdit4(type->name.c_str(), glm::value_ptr(type->Value),0.01);
			}

			if (field->getType() == MaterialDataType::TexturePath)
			{
				TexturePath* type = (TexturePath*)field.get();
				ImGui::Text(type->name.c_str());
				ImGui::Image(reinterpret_cast<ImTextureID>(texasset.GetAsset()->GetID()), { 100,100 });
				
			}
		}
		ImGui::End();

		ImGui::Begin("debug");
		ImGui::Text("texture cache count : %i", Application::GetInstance().assetManager->GetTextureCount());
		
		ImGui::End();
		_HierachyPanel->OnUpdate();
		EndImGui();

	}
	void EditorLayer::BeginImgui()
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
	}
	void EditorLayer::EndImGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void EditorLayer::OnDestroy()
	{
	}

	void EditorLayer::onEvent(Event& e)
	{
		


	}

	void EditorLayer::onMouseWheelEvent(MouseWheelEvent& e)
	{
		
	}
}