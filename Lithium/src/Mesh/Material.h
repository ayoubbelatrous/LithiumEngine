#pragma once
#include "Renderer/Shader.h"
#include "Core/Base.h"

namespace Lithium
{
	class Material
	{
	public:
		void SetShader(Ref<Shader> shader);
		Ref<Shader> GetShader();
	private:
		Ref<Shader> _Shader;
	};
}
