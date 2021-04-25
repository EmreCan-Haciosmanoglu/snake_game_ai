#pragma once
#include "Can.h"
#include "../../temps/NeuralNetwork.h"
class Game;

class Play {
public:
	Play(Game* game, glm::vec2 direction, int health);
	Play(Game* game, glm::vec2 direction, int health, NeuralNetwork* brain);
	~Play();

	void Tick();
	bool Update();
	void Draw(glm::vec2 offset);

	inline int GetPoints() const { return m_Points; }
	inline NeuralNetwork* GetBrain() { return m_Brain; }
	inline bool IsDeath() const { return m_Health <= 0 || b_IsDeath; }

	inline void SetBrain(NeuralNetwork* brain) { m_Brain = brain; }
	inline void SetDeath(bool isDeath) { b_IsDeath = isDeath; }
private:
	glm::vec2 GetNewApplePosition();
public:
	float m_State[12][12] = {
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f},
		{0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f}
	};
	int m_Points = 0;
	int m_Health;

	bool b_IsDeath = false;

	std::vector<glm::vec2> m_Snake;
	
	glm::vec2 m_CurrentDirection;
	glm::vec2 m_ApplePosition;

	Game* m_Game;
	NeuralNetwork* m_Brain;
};