#pragma once


namespace Lithium
{
	template<typename T>
	class Handle
	{
		T _Asset;
	public:
		Handle(const std::string& path)
			:Path(path)
		{}
		Handle() = default;
		~Handle()
		{
			*ref -= 1;
			deleteCallBack();
		}
		
		std::string Path;
		uint32_t Id;
		
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

