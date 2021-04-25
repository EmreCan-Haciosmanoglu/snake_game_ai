#pragma once
#include "Can.h"
#include "Play.h"

class Game {
public:
	Game(unsigned int playCount);
	~Game();

	void DrawBestPlay();
	void DrawThat(int index);
	void Tick(float ts);

	inline void PlayerDeath() { m_PlayLeft--; }

public:
	glm::vec4 m_RedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_WhitishColor = { 0.9f, 0.9f, 0.9f, 1.0f };
	glm::vec4 m_BlackishColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec2 m_Directions[4] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
	glm::vec2 m_Offset = { -9.0f, +3.0f };
	glm::vec2 m_Offset2 = { +8.0f, +3.0f };
	unsigned int m_PlayCount;
	unsigned int m_PlayLeft;
	std::vector<float> m_GenerationData;
	Play** m_Plays;
};