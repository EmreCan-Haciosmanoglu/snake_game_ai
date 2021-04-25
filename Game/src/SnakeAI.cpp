#include "canpch.h"
#include "SnakeAI.h"

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>  


SnakeAI::SnakeAI()
	:Layer("Snake Game")
	, m_CameraController(1920.0f / 1080.0f)
	, m_Game(new Game(100))
{
	float m_AspectRatio = 1920.0f / 1080.0f;
	float m_ZoomLevel = 10.0f;
	m_CameraController.GetCamera().SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

	m_Params.Position = { -8.5f, +1.5f, -0.2f };
	m_Params.Size = { 12.0f, 12.0f };
	m_Params.TintColor = { 0.85f, 0.85f, 0.85f, 1.0f };
}

void SnakeAI::OnUpdate(Can::TimeStep ts)
{
	//m_CameraController.OnUpdate(ts);
	m_Game->Tick(ts);

	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();


	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Can::Renderer2D::DrawQuad(m_Params);
	m_Game->DrawBestPlay();

	Can::Renderer2D::EndScene();
}

void SnakeAI::OnEvent(Can::Event::Event & event)
{
	m_CameraController.OnEvent(event);
}

void SnakeAI::OnImGuiRender()
{
	ImGui::Begin("Generation");
	int size = m_Game->m_GenerationData.size();
	for (int i = size-1; i >= 0; i--)
	{
		ImGui::Text("Generation-%d: %.1f", i, m_Game->m_GenerationData[i]);
	}
	ImGui::End();
}