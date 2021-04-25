#include "canpch.h"
#include "SnakeTake2.h"

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>  


SnakeAI2::SnakeAI2()
	:Layer("Snake Game 2")
	, m_CameraController(1920.0f / 1080.0f)
	, m_Game(new Game2(populationSize))
{
	float m_AspectRatio = 1920.0f / 1080.0f;
	float m_ZoomLevel = 20.0f;
	m_CameraController.GetCamera().SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

}

void SnakeAI2::OnAttach() {}
void SnakeAI2::OnDetach() {}

void SnakeAI2::OnUpdate(Can::TimeStep ts)
{
	//m_CameraController.OnUpdate(ts);
	m_Game->Tick(ts);

	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();


	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Can::Renderer2D::DrawQuad({ -14.5f, +1.5f, -0.2f }, { 12.0f, 12.0f }, { 0.85f, 0.85f, 0.85f, 1.0f });
	m_Game->DrawBestPlay();

	Can::Renderer2D::EndScene();
}

void SnakeAI2::OnEvent(Can::Event::Event & event)
{
	m_CameraController.OnEvent(event);
}



void SnakeAI2::OnImGuiRender()
{
	ImGui::Begin("Health", 0, 1);
	ImGui::SliderInt("Health", &(m_Game->m_StartHealth), 100, 400);
	ImGui::End();

	ImGui::Begin("Generation", 0, 2);
	size_t size = m_Game->m_GenerationData.size();
	for (int i = size - 1; i >= 0; i--)
	{
		ImGui::Text("Generation-%d: %.1f", i, m_Game->m_GenerationData[i]);
	}
	ImGui::End();
}