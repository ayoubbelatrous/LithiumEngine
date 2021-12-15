#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include "glm.hpp"
#include "AssetManager/AssetManager.h"


namespace Lithium
{
	class SpriteEditor
	{
	public:
		void OnCreate();
		void SetTexture(const Ref<Texture>& texture);
		void OnUpdate();
		void Open()
		{
			_Open = true;
		}
		void Close()
		{
			_Open = false;
		}
	private:
		bool _Open = false;
		glm::vec2 cellsize;
		Ref<Texture> _Texture;
		Ref<TextureData> _TextureData;
		bool hasMetadata = false;
	};
}