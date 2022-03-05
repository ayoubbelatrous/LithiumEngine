#include "lipch.h"
#include "Script/ScriptClass.h"

namespace Lithium
{
	
	ScriptClass::ScriptClass(MonoClass* monoclass)
		:m_MonoClass(monoclass)
	{

	}

	MonoClass* ScriptClass::GetClassPtr()
	{
		return m_MonoClass;
	}

}