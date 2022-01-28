#include "lipch.h"

#include "Renderer.h"
#include <glad/glad.h>

namespace Lithium
{
	
	void RendererCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RendererCommand::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererCommand::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererCommand::Draw(uint32_t count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

}