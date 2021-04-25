#pragma once
#include "Can.h"
#include "Can/Renderer/Renderer2D.h"
#include "imgui.h"

class SandBox2DLayer : public Can::Layer::Layer
{
public:
	SandBox2DLayer();
	virtual ~SandBox2DLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	Can::Camera::OrthographicCameraController m_CameraController;

	Can::Ref<Can::Shader> m_SquareShader;
	Can::Ref<Can::VertexArray> m_SquareVA;

	Can::Ref<Can::Texture2D> m_Tex;
	Can::Ref<Can::Texture2D> m_Man;
	Can::Ref<Can::Texture2D> m_Man2;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.0f, 1.0f, 0.0f, 1.0f };

};