#pragma once
#include "SceneRenderer/SceneRendererData.h"
#include "SceneRenderer/Mesh/Mesh.h"
#include "Core/UUID.h"

namespace Lithium::Rendering
{
	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		void Init();
		void Init_RT();
		void OnUpdate();
		void OnRender_RT();
		UUID Submit(const MeshResource& meshRecource);
	private:
		Ref<Data::SceneRendererData> m_RendererData;
	};
}