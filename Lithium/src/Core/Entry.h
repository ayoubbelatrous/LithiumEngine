#pragma once
#include "Application.h"
#include "Debug/Profiler.h"




extern Lithium::Application* Lithium::CreateApp();
int main()
{
	auto app = Lithium::CreateApp();

	app->Run();
	delete app;


}
