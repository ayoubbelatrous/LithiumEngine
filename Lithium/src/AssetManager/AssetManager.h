#pragma once
#include "COre/Base.h"
#include <unordered_map>
#include "Renderer/Texture.h"

namespace Lithium
{
	class AssetMananger
	{
	public:
		template<typename T>
		T LoadAsset(const std::string& path);
		template<typename T>
		T GetByHandle(uint32_t id);
		bool IsLoaded(uint32_t id);
		void GenerateTextureMetadata();
	private:
		std::unordered_map<uint32_t , Ref<Texture>> _TextureCache;
		std::unordered_map<std::string,uint32_t> _Cache;
		int Ptr = 0;
	};

}