#include "lipch.h"
#include "Script/ScriptClassField.h"


namespace Lithium
{

	void ScriptClassField::CheckIfSubClassOfComponent()
	{
		MonoType* type = mono_field_get_type(_fieldHandle);
		MonoClass * klass = mono_class_from_mono_type(type);
		if (mono_class_is_subclass_of(klass, _ComponentClass, false))
		{
			CORE_LOG("field : " << name << " is component");
			IsSubClassOfComponent = true;
		}
	}

}