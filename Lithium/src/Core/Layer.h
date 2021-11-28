#pragma once

namespace Lithium
{
	class Layer
	{
	public:
		Layer(const std::string& name);

		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}

		std::string& GetName() { return _name; }
		std::string _name = "TEST";
	};
}