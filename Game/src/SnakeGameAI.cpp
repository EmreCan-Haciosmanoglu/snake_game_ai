#include "canpch.h"
#include "Can.h"

#include "Can/EntryPoint.h"
#include "SnakeAI.h"
#include "SnakeTake2.h"

class SnakeGameAI : public Can::Application
{
public:
	SnakeGameAI(const Can::WindowProps& props)
		:Application(props)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Light.ttf", 24.0f);

		//PushLayer(new SnakeAI());
		PushLayer(new SnakeAI2());
	}

	~SnakeGameAI()
	{

	}
};

Can::Application* Can::CreateApplication(const Can::WindowProps& props)
{
	return new SnakeGameAI(props);
}