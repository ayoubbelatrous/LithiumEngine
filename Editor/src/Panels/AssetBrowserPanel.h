#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include <filesystem>
#include <functional>
#include "../EditorEvents.h"
#include <vector>
#include <iostream>

namespace Lithium {
	using EventCallback = std::function<void(Event&)>;
	class AssetBrowserPanel
	{

	public:
		void OnCreate();
		void OnUpdate();
		void OnProjectChange();
		void SetEventCallback(const EventCallback& callbackfn)
		{
			callback = callbackfn;
		}
		void Refresh();
		std::filesystem::path& GetCurrentPath()
		{
			return currentpath;
		}
	private:
		Ref<Texture> _FolderIcon;
		Ref<Texture> _FileIcon;
		std::filesystem::path currentpath;
		EventCallback callback;
		std::vector<std::filesystem::directory_entry> Cache;
	};
}