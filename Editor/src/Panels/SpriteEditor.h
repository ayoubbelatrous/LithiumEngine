#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"

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
		Ref<Texture> _Texture;
	};
}