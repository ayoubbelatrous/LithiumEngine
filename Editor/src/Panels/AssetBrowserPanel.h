#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include <filesystem>

namespace Lithium {
	class AssetBrowserPanel
	{
	public:
		void OnCreate();
		void OnUpdate();
	private:
		Ref<Texture> _FolderIcon;
		Ref<Texture> _FileIcon;
		std::filesystem::path currentpath;
	};
}