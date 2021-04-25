#include "canpch.h"
#include "SandBox3D.h"


SandBox3D::SandBox3D()
	: Layer("3D Layer")
	, m_CameraController(45.0f, 16.0f / 9.0f, 0.1f, 30.0f)
{
}

void SandBox3D::OnAttach()
{
}
void SandBox3D::OnDetach()
{
}

void SandBox3D::OnUpdate(Can::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer3D::BeginScene(m_CameraController.GetCamera());

	//Can::Renderer3D::DrawCube({ 0.0f, 0.0f, -4.0f }, { 0.5f, 0.5f, 0.5f });

	Can::Renderer2D::EndScene();
}

void SandBox3D::OnEvent(Can::Event::Event & event)
{
	m_CameraController.OnEvent(event);
}

void SandBox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	bool isChanged = ImGui::SliderInt("Resolution", &m_Resolution,1,25);
	//if (isChanged)
	//	Can::Renderer3D::UpdateSphere(m_Resolution);

	ImGui::End();
}
