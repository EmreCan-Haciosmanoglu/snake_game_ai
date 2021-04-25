#pragma once
#include "Can.h"
#include "imgui.h"

class SandBox3D : public Can::Layer::Layer
{
public:
	SandBox3D();
	virtual ~SandBox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	int m_Resolution = 4;
	Can::Camera::Controller::Perspective m_CameraController;

	Can::Ref<Can::Shader> m_CubeShader;
	Can::Ref<Can::VertexArray> m_CubeVA;
};