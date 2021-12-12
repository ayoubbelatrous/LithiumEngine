#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include <filesystem>
#include <functional>
#include "../EditorEvents.h"

namespace Lithium {
	using EventCallback = std::function<void(Event&)>;
	class AssetBrowserPanel
	{

	public:
		void OnCreate();
		void OnUpdate();
		void SetEventCallback(const EventCallback& callbackfn)
		{
			callback = callbackfn;
		}
	private:
		Ref<Texture> _FolderIcon;
		Ref<Texture> _FileIcon;
		std::filesystem::path currentpath;
		EventCallback callback;
	};
}