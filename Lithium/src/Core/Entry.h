#pragma once
#include "Core.h"
#include "Application.h"





extern Lithium::Application* Lithium::CreateApp();
int main()
{
	auto app = Lithium::CreateApp();
	app->Run();
	delete app;
}

