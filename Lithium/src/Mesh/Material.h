#pragma once
#include "Renderer/Shader.h"

namespace Lithium
{
	class Material
	{
		Ref<Shader> shader;
	public:
		Material();
		~Material();
		void SetShader(const Ref<Shader>& shader);
		void BindShader();
		
	};
}


