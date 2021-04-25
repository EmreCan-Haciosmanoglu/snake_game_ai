#pragma once
#include "Can.h"
#include "../temps/NeuralNetwork.h"

#include "imgui.h"

#include "SnakeGame2/Game2.h"

class SnakeAI2 : public Can::Layer::Layer
{
public:
	SnakeAI2();
	virtual ~SnakeAI2() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Can::Camera::OrthographicCameraController m_CameraController;

	int populationSize = 300;
	Game2* m_Game;

	Can::DrawQuadParameters m_Params = Can::DrawQuadParameters();
};