#pragma once
#include "AssetManager/Asset.h"

namespace Lithium
{
	class SpriteEditor
	{
		
	public:
		void OnCreate();
		void OnUpdate(bool* open);
		/*void SetEventCallback(const EventCallback& cb)
		{
			callback = cb;
		}*/
		void SetAsset(Asset asset)
		{
			m_CurrentAsset = asset;
		}
	private:	
		Asset m_CurrentAsset;
		glm::vec2 cellsize = glm::vec2(1.0f);
	};
}
