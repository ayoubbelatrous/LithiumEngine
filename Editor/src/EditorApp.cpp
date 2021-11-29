#include "Lithium.h"
#include "EditorLayer.h"
#include "Core/Entry.h"


class Editor : public Lithium::Application
{

	public:
		Editor()
			:Application()
		{
			CORE_LOG("INIT APP");
			PushLayer(new Lithium::EditorLayer());

		}

		~Editor()
			
		{

		}

};

Lithium::Application* Lithium::CreateApp() {

	
	return new Editor();
}
