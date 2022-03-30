#include "Lithium.h"
#include "Core/Entry.h"
#include "RuntimeLayer.h"

class Runtime : public Lithium::Application
{

public:
	Runtime()
		:Application()
	{
		PushLayer(new Lithium::RuntimeLayer());
	}

	~Runtime()
	{

	}

};

Lithium::Application* Lithium::CreateApp() {
	return new Runtime();
}
