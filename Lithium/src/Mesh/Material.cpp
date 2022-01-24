#include "lipch.h"
#include "Material.h"

namespace Lithium
{

	void Material::SetShader(Ref<Shader> shader)
	{
		_Shader = shader;
	}

	Ref<Shader> Material::GetShader()
	{
		return _Shader;
	}

}