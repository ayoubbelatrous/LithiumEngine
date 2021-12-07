#include "lipch.h"
#include "Shader.h"
#include "glad/glad.h"

namespace Lithium
{
	Shader::Shader()
	{
		id = 0;
	}

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	Shader::Shader(const std::string& path)
	{


		enum class ShaderType
		{
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1
		};
		ShaderType shadertype = ShaderType::NONE;


		std::ifstream stream(path);
		std::string line;
		std::stringstream ss[2];
		while (std::getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					shadertype = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					shadertype = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)shadertype] << line << "\n";
			}
		}
		id = CreateShader(ss[0].str(), ss[1].str());
		std::cout << "Loaded Shader: " << "'" << path << "'" << std::endl;
		stream.close();
	}


	unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		//error handling

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(sizeof(char) * length);
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "shader error" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	unsigned int Shader::CreateShader(const std::string& vertexshader, const std::string& fragmentshader)
	{
		unsigned int Program = glCreateProgram();

		unsigned int vs = CompileShader(vertexshader, GL_VERTEX_SHADER);
		unsigned int fs = CompileShader(fragmentshader, GL_FRAGMENT_SHADER);

		glAttachShader(Program, vs);
		glAttachShader(Program, fs);
		glLinkProgram(Program);
		glValidateProgram(Program);


		glDeleteShader(vs);
		glDeleteShader(fs);
		return Program;
	}


	void Shader::Bind()
	{
		glUseProgram(id);
	}

	Ref<Shader> Shader::Load(const std::string& path)
	{

		return CreateRef<Shader>(path);
	}


	unsigned int Shader::GetID()
	{
		return id;
	}



	int Shader::GetLocation(const std::string& name)
	{
		if (unifromCache.find(name) != unifromCache.end())
		{
			return unifromCache[name];
		}
		int location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
		{
			unifromCache[name] = location;
		}
		return location;
	}
	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector)
	{
		int location = GetLocation(name);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}
	void Shader::SetUniform1i(const std::string& name, int value)
	{
		int location = GetLocation(name);
		glUniform1i(location, value);
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
	{
		int location = GetLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetUniformiv(const std::string& name, int *vector)
	{

		int location = GetLocation(name);
		
		glUniform1iv(location, 18, vector);

	}

}