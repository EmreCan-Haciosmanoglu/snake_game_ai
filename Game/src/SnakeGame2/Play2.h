#pragma once
#include "Can.h"
#include "../../temps/NeuralNetwork.h"
class Game2;

class Play2 {
public:
	Play2(Game2* game, glm::vec2 direction, int health, NeuralNetwork* brain, int dimension, int inputSize);
	~Play2();

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
	int** m_State;

	int m_Points = 0;
	int m_Health;
	int m_Dimension;
	int m_InputSize;
	int m_DirectionIndex = 0;

	bool b_IsDeath = false;

	std::vector<glm::vec2> m_Snake;
	
	glm::vec2 m_CurrentDirection;
	glm::vec2 m_ApplePosition;

	Game2* m_Game;
	NeuralNetwork* m_Brain;
};