#pragma once
#include "COre/Base.h"
#include "Renderer/Texture.h"


namespace Lithium {
	class AssetBrowserPanel
	{
	public:
		void OnCreate();
		void OnUpdate();
	private:
		Ref<Texture> _FolderIcon;
		Ref<Texture> FileIcon;
	};
}