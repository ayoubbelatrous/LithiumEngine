#pragma once




namespace Lithium::Scripting
{
	class MonoCompiler
	{
		static MonoMethod* _method;
	public:
		
		static bool Invoke(const Ref<MonoServer>& server);
	};
}