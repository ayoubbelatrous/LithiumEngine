#pragma once
#include "glm.hpp"
#include "Core/Base.h"
#include <unordered_map>
#include <string>

namespace Lithium
{
	struct ShaderDataInfo
	{
		ShaderDataInfo(size_t buffsize, uint32_t datatype, int size, const std::string& name)
			:BufferSize(buffsize), DataType(datatype), Size(size), Name(name)
		{}
		ShaderDataInfo()
		{}
		size_t BufferSize;
		uint32_t DataType;
		int Size;
		std::string Name;

	};
	class Shader
	{
	private:
		unsigned int id = 0;
		std::unordered_map<std::string, int> unifromCache;
		static unsigned int CompileShader(const std::string& source, unsigned int type);
		static unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
		std::string _Path;
	public:
		Shader(const std::string& path);
		Shader();
		~Shader();
		unsigned int  GetID();
		void Bind();
		int GetLocation(const std::string& name);
		void SetUniform4f(const std::string& name, const glm::vec4& vector);
		void SetUniform3f(const std::string& name, const glm::vec3& vector);
		void SetUniform1i(const std::string& name, int value);
		void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
		void SetUniformiv(const std::string& name, int vector[]);

		int GetAttribCount();
		std::string GetPath()
		{
			return _Path;
		}
		ShaderDataInfo GetAttribute(uint32_t index);
		static Ref<Shader> Load(const std::string& path);
	};
}