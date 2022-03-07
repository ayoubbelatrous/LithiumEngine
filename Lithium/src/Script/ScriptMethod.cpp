#include "lipch.h"
#include "Script/ScriptMethod.h"
#include "Core/Application.h"

namespace Lithium
{

	ScriptMethod::ScriptMethod(MonoMethod* m_Method, MonoObject* m_Object)
		:m_MonoMethod(m_Method),m_MonoObject(m_Object)
	{}

	void* ScriptMethod::Invoke(void* params)
	{
		MonoObject* exception = nullptr;
		mono_runtime_invoke(m_MonoMethod, m_MonoObject, &params, &exception);
		if (exception != nullptr)
		{
			Application::Get().Monoserver->ForwardMonoException(exception);
		}
		return nullptr;
	}

}