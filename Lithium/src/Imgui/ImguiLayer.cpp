#include "lipch.h"
#include "ImguiLayer.h"
#include "Core/Application.h"
#include <../imgui/example/imgui_impl_glfw.h>
#include <../imgui/example/imgui_impl_opengl3.h>
#include "GLFW/glfw3.h"

namespace Lithium
{

	void GUIlayer::OnCreate()
	{
		Application &app = Application::GetInstance();
		//initilize imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();

		//imgui opengl glfw3
		ImGui_ImplGlfw_InitForOpenGL(app.GetWindow().GetglfwWindowptr(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
		}

	void GUIlayer::OnUpdate()
	{
		
	}

	void GUIlayer::OnDestroy()
	{
	}

}
