#pragma once


namespace Lithium
{
	template<typename T>
	class Asset
	{
		T _Asset;
	public:
		Asset(const std::string& path)
			:Path(path)
		{}
		Asset() = default;
		~Asset()
		{
			*ref -= 1;
			deleteCallBack();
		}
		
		uint32_t Id;
		std::string Path;
		
		T& GetAsset()
		{
			return _Asset;
		}

		void SetAsset(const T& asset)
		{
			_Asset = asset;
		}
		
		uint32_t* ref;

		std::function<void()> deleteCallBack;
		void SetDeleteCallBack(std::function<void()> callback)
		{
			deleteCallBack = callback;
		}
	};
}

