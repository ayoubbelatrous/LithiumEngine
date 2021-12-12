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
	private:

		Ref<Texture> _Texture;
	};
}