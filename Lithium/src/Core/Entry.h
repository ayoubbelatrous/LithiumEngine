#pragma once
#include "Application.h"
#include "Debug/Profiler.h"




extern Lithium::Application* Lithium::CreateApp();
int main()
{
	auto app = Lithium::CreateApp();

	LT_BEGIN_SESSION("Runtime", "Lithium-Runtime.json");
	app->Run();
	LT_END_SESSION();
	delete app;
}
