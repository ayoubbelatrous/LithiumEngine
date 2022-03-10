#pragma once
#include "Asset.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"
#include "Renderer/Texture.h"
#include "Core/Timer.h"
namespace Lithium
{
	class AssetManager
	{
	private:
		std::unordered_map<uint32_t, Ref<Material>> _MaterialCache;
		std::unordered_map<uint32_t, Ref<Mesh>> _MeshCache;
		std::unordered_map<uint32_t, Ref<Texture>> _TextureCache;
		std::unordered_map<std::string,uint32_t> _PathTable;

		std::unordered_map<uint32_t, uint32_t> _TextureRefrenceCount;
	public:
		AssetManager()
		{
			
		}
		template<typename T>
		Asset<T> LoadAsset(const std::string& path);
		void CheckForRefrences();
		uint32_t GetTextureCount() { return _TextureCache.size(); };
		
	};
}