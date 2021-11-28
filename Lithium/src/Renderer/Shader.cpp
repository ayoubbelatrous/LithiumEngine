#include "Shader.h"


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
	std::cout << "Loaded Shader: " << "'" << path << "'" <<std::endl;
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

Shader* Shader::Load(const std::string& path)
{
	
	return new Shader(path);
}


unsigned int Shader::GetID()
{
	return id;
}