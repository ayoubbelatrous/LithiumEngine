#pragma once
#include "glm.hpp"
#include "Core/Base.h"
#include <unordered_map>
#include <string>

namespace Lithium
{
	class Shader
	{
	private:
		unsigned int id = 0;
		std::unordered_map<std::string, int> unifromCache;
		static unsigned int CompileShader(const std::string& source, unsigned int type);
		static unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
	public:
		Shader(const std::string& path);
		Shader();
		~Shader();
		unsigned int  GetID();
		void Bind();
		int GetLocation(const std::string& name);
		void SetUniform4f(const std::string& name, const glm::vec4& vector);
		void SetUniform1i(const std::string& name, int value);
		void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
		void SetUniformiv(const std::string& name, int vector[]);
		static Ref<Shader> Load(const std::string& path);
	};
}