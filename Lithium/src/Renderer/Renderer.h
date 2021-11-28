#pragma once
#include "GL/glew.h"
#include <iostream>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#define ASSERT(x) \
	if (!(x))\
		__debugbreak();

#define GLCall(x)\
	ClearGLError();\
	x;             \
	ASSERT(LogGLCall(#x, __FILE__, __LINE__))

void ClearGLError();
bool LogGLCall(const char* name, const char* file, int line);


namespace Renderer{

	static void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	static void ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	static void Draw(int count)
	{
		
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}


}


namespace Renderer2D
{
	void Init();
	void StartBatch();
	void NextBatch();
	void BeginScene(glm::mat4 proj, glm::mat4 view);
	void EndScene();
	void Flush();
	void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
}