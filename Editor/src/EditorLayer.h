#pragma once
#include "Lithium.h"
class EditorLayer : public Lithium::Layer
{
public:
	EditorLayer()
		:Layer("hi")
	{

	}
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;
};


