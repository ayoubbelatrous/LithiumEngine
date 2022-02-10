#include "EditorLayer.h"

#include "ImGuizmo.h"
#include "Input/Input.h"
#include "Core/Math.h"
#include "Font/Font.h"



namespace Lithium
{
	
	void EditorLayer::OnCreate()
	{

		FrameBufferAttachmentDescriptor mainframebufferdescryptor(
			{
				FramebufferTextureFormat::RGBA8,
				FramebufferTextureFormat::Depth
			}
		);
		_MainFramebuffer = CreateRef<FrameBuffer>(mainframebufferdescryptor);
		_MainFramebuffer->resize(1000, 1000);

		_MainScene = CreateRef<Scene>();
	}
	void EditorLayer::OnUpdate()
	{
		_MainFramebuffer->resize(ViewportSize.x, ViewportSize.y);
		_MainFramebuffer->Bind();
		_MainScene->onEditorUpdate();
		RendererCommand::ClearColor(glm::vec4(1));
		RendererCommand::Clear();
		_MainFramebuffer->UnBind();
		_MainFramebuffer->BindTexture(1, 0);
		RenderImgui();
	}

	void EditorLayer::RenderImgui()
	{
		BeginImgui();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("scene");
		ViewportSize.x = ImGui::GetContentRegionAvail().x;
		ViewportSize.y = ImGui::GetContentRegionAvail().y;
		ImGui::Image(reinterpret_cast<ImTextureID>(_MainFramebuffer->GetColorAttachmentID()),ImVec2(ViewportSize.x,ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

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
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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