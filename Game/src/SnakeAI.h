#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

#include "imgui.h"

#include "SnakeGame/Game.h"

class SnakeAI : public Can::Layer::Layer
{
public:
	SnakeAI();
	virtual ~SnakeAI() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Can::Camera::OrthographicCameraController m_CameraController;

	glm::vec2 m_Offset = { -7.0f, +3.0f };
	int PlayerNum = 0;

	Game* m_Game;
};