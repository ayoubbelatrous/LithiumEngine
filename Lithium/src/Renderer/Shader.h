#pragma once
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "glm/glm.hpp"
#include <unordered_map>

class Shader
{
private:
	unsigned int id = 0;
	std::unordered_map<std::string, GLint> unifromCache;
	static unsigned int CompileShader(const std::string& source, unsigned int type);
	static unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
public:
	Shader(const std::string& path);
	Shader();
	~Shader();
	unsigned int  GetID();
	void Bind();
	int GetLocation(const std::string& name)
	{
		if (unifromCache.find(name) != unifromCache.end())
		{
			return unifromCache[name];
		}
		int location = glGetUniformLocation(id,name.c_str());
		if (location != -1)
		{
			unifromCache[name] = location;
		}
		return location;
	}
	void SetUniform4f(const std::string& name,const glm::vec4& vector)
	{
		int location = GetLocation(name);
		GLCall(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
	}
	void SetUniform1i(const std::string& name, int value)
	{
		int location = GetLocation(name);
		GLCall(glUniform1i(location, value));
	}

	void SetUniformMat4f(const std::string& name, const glm::mat4& mat)
	{
		int location = GetLocation(name);
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
	}
	static Shader* Load(const std::string& path);
};