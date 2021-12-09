#include "Lithium.h"
#include "Core/Entry.h"
#include "RuntimeLayer.h"

class Editor : public Lithium::Application
{

public:
	Editor()
		:Application()
	{
		CORE_LOG("INIT RUNTIME");
		PushLayer(new Lithium::RuntimeLayer());
	}

	~Editor()
	{

	}

};

Lithium::Application* Lithium::CreateApp() {
	return new Editor();
}
