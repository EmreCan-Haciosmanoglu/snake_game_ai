#pragma once
#include "Can.h"
#include "Play2.h"

class Game2 {
public:
	Game2(unsigned int playCount);
	~Game2();

	void DrawBestPlay();
	void Tick(float ts);

	inline void PlayerDeath() { m_PlayLeft--; }
	inline int* GetNodes() { return new int[4]{ (int)m_InputSize , 20 , 20, 3 }; }
public:
	glm::vec4 m_RedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_WhitishColor = { 0.9f, 0.9f, 0.9f, 1.0f };
	glm::vec4 m_BlackishColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	glm::vec2 m_Directions[4] = { {0,1}, {-1,0}, {0,-1}, {1,0} };

	glm::vec2 m_Offset = { -15.0f, +3.0f };

	int m_StartHealth;
	unsigned int m_InputSize;
	int m_Dimension;
	unsigned int m_PlayCount;
	unsigned int m_PlayLeft;
	std::vector<float> m_GenerationData;
	Play2** m_Plays;
};