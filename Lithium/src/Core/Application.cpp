#include "lipch.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "glm/glm.hpp"
#include "Math.h"
#include "Log.h"

namespace Lithium
{
	Application* Application::instance = nullptr;

	void Application::SetApplicationProperties(ApplicationProperties props)
	{
		m_ApplicationProps = props;
		OnProjectPropertiesChanged();
	}

	ApplicationProperties Application::GetApplicationProperties()
	{
		return m_ApplicationProps;
	}

	void Application::ReloadAssetManager()
	{
		assetManager = CreateRef<AssetManager>();
	}

	void Application::OnProjectPropertiesChanged()
	{
		if (!m_ApplicationProps.WorkingDirectory.empty())
		{
			std::filesystem::current_path(m_ApplicationProps.WorkingDirectory);
		}
	}

	Application::Application()
	{
		Log::Init();
		_Window = CreateScope<Window>();
		_Window->Init();
#ifndef STATIC_RUNTIME
		_ImguiLayer = new GUIlayer();
		PushOverlay(_ImguiLayer);
#endif
		_Window->SetAppEventCallback(BIND_EVENT(Application::OnEvent));
		Math::Random::Init();
		assetManager = CreateRef<AssetManager>();
		sceneManager = CreateRef<SceneManager>();
		Monoserver = CreateRef<MonoServer>();

	}
	Application::~Application()
	{
		for (Layer* layer : _Stack)
		{
			layer->OnDestroy();
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		instance = this;
		_Stack.PushLayer(layer);
		layer->OnCreate();
	}

	void Application::PushOverlay(Layer* layer)
	{
		instance = this;
		_Stack.PushOverlay(layer);
		layer->OnCreate();
	}

	void Application::Run()
	{


		while (_running)
		{
			double time = (double)glfwGetTime();
			m_DeltaTime = time - m_PreviousTime;
			m_PreviousTime = time;

			if (m_Minimized == false)
			{
				for (Layer* layer : _Stack)
				{
					layer->OnUpdate();
				}
				_ImguiLayer->Begin();
				{
					for (Layer* layer : _Stack)
						layer->OnImguiRender();
				}
				_ImguiLayer->End();
			}
			_Window->OnUpdate();

		}
	}


	void Application::OnEvent(Event& e)
	{


		for (auto it = _Stack.rbegin(); it != _Stack.rend(); ++it)
		{
			if (e._Handeled)
				break;
			(*it)->onEvent(e);
		}
		if (e.GetEventType() == EventType::WindowClose)
		{
			_running = false;
		}
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& resizeevent = static_cast<WindowResizeEvent&>(e);
			int height = resizeevent.height;
			int width = resizeevent.width;
			if (width == 0 && height == 0)
			{
				m_Minimized = true;
			}
			else
			{
				m_Minimized = false;
			}

		}
	}

	void Application::SceneOnEvent(Event& e)
	{
		for (auto it = _Stack.rbegin(); it != _Stack.rend(); ++it)
		{
			if (e._Handeled)
				break;
			(*it)->onEvent(e);
		}
	}

	void Application::SetVsync(bool vsync)
	{
		_Window->SetVsync(vsync);
	}

	bool Application::IsVsync()
	{
		return _Window->IsVsync();
	}

	Window& Application::GetWindow()
	{
		return *_Window;
	}
}
