#pragma once


namespace Lithium
{
	class MonoClass
	{
	public:
		MonoClass();
		~MonoClass();
		void Reload();
	private:
		MonoClass* _Klass;
		std::string _Name;
		std::string _Path;
	};
}
