#pragma once



namespace Lithium
{
	class Panel
	{
	public:
		virtual void onCreate() = 0;
		virtual void onUpdate() = 0;
	};
}