#include "lithium.h"

class EditorLayer : public Lithium::Layer
{
public:
	EditorLayer()
		:Layer("hi")
	{
		
	}


	void OnCreate() override
	{
		CORE_LOG("init");
	}

	void OnUpdate() override
	{
		
	}
	void OnDestroy() override
	{
		CORE_LOG("hi");
	}
};



class Editor : public Lithium::Application
{

	public:
		Editor()
			:Application()
		{
			CORE_LOG("who");
			PushLayer(new EditorLayer());
		}

		~Editor()
			
		{

		}

};

Lithium::Application* Lithium::CreateApp() {

	
	return new Editor();
}
