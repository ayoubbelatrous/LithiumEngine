#include "lipch.h"

#include "Renderer.h"
#include <glad/glad.h>

namespace Lithium
{
	
	void RendererCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererCommand::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererCommand::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererCommand::DrawLines(uint32_t count)
	{
		glDrawArrays(GL_LINES, 0, count);
	}

	void RendererCommand::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void RendererCommand::Draw(uint32_t count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

}