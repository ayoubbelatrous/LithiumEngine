#include "EditorLayer.h"
#include "imgui.h"
#include <../imgui/example/imgui_impl_glfw.h>
#include <../imgui/example/imgui_impl_opengl3.h>

namespace Lithium
{
	void EditorLayer::OnCreate()
	{
		LT_PROFILE_FUNCTION("init");
		float positions[] = {
			-0.5f,
			-0.5f,
			0.5f,
			-0.5f,
			0.5f,
			0.5f,
			-0.5f,
			0.5f,
		};
		unsigned int index[] = {
			0, 1, 2,
			2, 3, 0};

		pos = glm::vec3(0);
		view = glm::mat4(0);
		view = model = glm::translate(glm::mat4(1), glm::vec3(1));

		proj = glm::ortho(-2.0, 2.0, -2.0, 2.0);
		model = glm::translate(glm::mat4(1), pos);
		tex = CreateRef<Texture>("assets/images/check.png");
		tex2 = CreateRef<Texture>("assets/images/eda.png");
		framebuffer = CreateRef<FrameBuffer>();
		framebuffer->Bind();
		framebuffer->resize(1280, 780);
		Renderer2D::Init();
	}

	void EditorLayer::OnUpdate()
	{

		LT_PROFILE_FUNCTION("update");
		

		
		framebuffer->Bind();
		RendererCommand::ClearColor(glm::vec4(0.25));
		RendererCommand::Clear();

		Renderer2D::BeginScene(proj, view);

		//Renderer2D::DrawQuad(model, glm::vec4(1));
		model = glm::translate(glm::mat4(1), pos);

	
	    model = glm::translate(glm::mat4(1), {0.0, 0.0, 0.0});
		Renderer2D::DrawQuad(model, glm::vec4(1.0, 0.0, 0.0, 1.0));
		Renderer2D::EndScene();
		framebuffer->UnBind();
		RenderImgui();
	}

	void EditorLayer::OnDestroy()
	{
	}

	void EditorLayer::onEvent(Event& e)
	{
	
		if (e.GetEventType() == EventType::KeyPress)
		{
			KeyEvent& key = static_cast<KeyEvent&>(e);



			if (key.GetKeyAction() == GLFW_PRESS)
			{
				CORE_LOG("pressed");
			}

			if (key.GetKeyAction() == GLFW_RELEASE)
			{
				CORE_LOG("released");
			}

			
			CORE_LOG(key.GetKeyMods());
			
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<KeyEvent>(BIND_EVENT(EditorLayer::onKeyEvent));
		}

	}

	void EditorLayer::onKeyEvent(KeyEvent& e)
	{
		CORE_LOG(e.GetKeyCode());
		CORE_LOG("HIHI");
	}

	void EditorLayer::RenderImgui()
	{
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

		ImGui::Begin("hi");
		
		ImGui::Image((void*)(intptr_t)framebuffer->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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
