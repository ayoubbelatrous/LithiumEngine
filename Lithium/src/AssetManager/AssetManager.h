#pragma once
#include "COre/Base.h"
#include <unordered_map>


namespace Lithium
{
	class AssetMananger
	{
	public:
		static void LoadAsset();
	private:
		static std::unordered_map<uint32_t, std::string> _Data;
	};
	static AssetMananger manager;
}