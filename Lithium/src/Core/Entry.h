#pragma once
#include "Application.h"
#include "Debug/Profiler.h"




extern Lithium::Application* Lithium::CreateApp();
int main()
{
	LT_BEGIN_SESSION("render loop", "results.log");
	auto app = Lithium::CreateApp();
	
	app->Run();
	LT_END_SESSION();
	delete app;

	
}

