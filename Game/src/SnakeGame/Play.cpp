#include "canpch.h"
#include "Play.h"
#include "Game.h"

Play::Play(Game* game, glm::vec2 direction, int health)
	: m_CurrentDirection(direction)
	, m_Health(health)
	, m_Game(game)
{
	int* nodes = new int[4]{ 12 * 12 + 2 , 50 , 30, 4 };
	m_Brain = new NeuralNetwork(nodes, 4, 0.05f);
	m_Snake.push_back({ 5, 4 });
	m_State[4][5] = -1;
	m_Snake.push_back({ 5, 5 });
	m_State[5][5] = -1;

	int x = std::rand() % 12;
	int y = std::rand() % 12;
	while (m_State[y][x] != 0.01f)
	{
		x = std::rand() % 12;
		y = std::rand() % 12;
	}
	m_ApplePosition = glm::vec2(x, y);
	m_State[y][x] = 1;
}

Play::Play(Game* game, glm::vec2 direction, int health, NeuralNetwork* brain)
	: m_CurrentDirection(direction)
	, m_Health(health)
	, m_Game(game)
	, m_Brain(brain)
{
	m_Snake.push_back({ 5, 4 });
	m_State[4][5] = -1;
	m_Snake.push_back({ 5, 5 });
	m_State[5][5] = -1;
	int x = std::rand() % 12;
	int y = std::rand() % 12;
	while (m_State[y][x] != 0.01f)
	{
		x = std::rand() % 12;
		y = std::rand() % 12;
	}
	m_ApplePosition = glm::vec2(x, y);
	m_State[y][x] = 1;
}

Play::~Play()
{
	delete m_Brain;
}

void Play::Tick()
{

	float state[12 * 12 + 2];

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			state[i * 12 + j] = (m_State[i][j]);
		}
	}
	state[12 * 12] = m_Snake.back().x;
	state[12 * 12 + 1] = m_Snake.back().y;

	int size = 12 * 12 + 2;
	Matrix* input = new Matrix(size, 1, state);
	Matrix* result = m_Brain->FeedForward(input);

	float A[4] = {
		result->data[0],
		result->data[1],
		result->data[2],
		result->data[3]
	};
	float* maxElement = std::max_element(A, A + 4);
	int dist = std::distance(A, maxElement);
	m_CurrentDirection = m_Game->m_Directions[dist];
	delete result;

	b_IsDeath = Update();
	if (b_IsDeath)
		m_Game->PlayerDeath();
}

bool Play::Update()
{
	m_Health--;
	m_Points++;

	glm::vec2 position = m_Snake.back();
	glm::vec2 nextPos = position + m_CurrentDirection;
	if (nextPos.x < 0 || nextPos.x > 11 || nextPos.y < 0 || nextPos.y > 11 || m_State[(int)nextPos.y][(int)nextPos.x] == -1)
	{
		return true;
	}
	glm::vec2 tailPos = m_Snake.front();
	m_State[(int)tailPos.y][(int)tailPos.x] = 0.01f;
	if (nextPos.x == m_ApplePosition.x && nextPos.y == m_ApplePosition.y)
	{
		m_Snake.insert(m_Snake.begin(), m_Snake.front());
		m_Health += 10;
		m_Points += 5;
		m_ApplePosition = GetNewApplePosition();
	}
	if (m_Health <= 0)
		return true;
	m_Snake.push_back(nextPos);
	m_State[(int)nextPos.y][(int)nextPos.x] = -1;
	m_Snake.erase(m_Snake.begin());
	return false;
}

void Play::Draw(glm::vec2 offset)
{
	for (int y = -7; y < 5; y++)
	{
		for (int x = -5; x < 7; x++)
		{
			switch ((int)m_State[y + 7][x + 5])
			{
			case 1:
				//Can::Renderer2D::DrawQuad({ x + offset.x, y + offset.y , 0.1f }, { 0.9f, 0.9f }, m_Game->m_RedColor);
				break;
			case -1:
				//Can::Renderer2D::DrawQuad({ x + offset.x, y + offset.y , 0.1f }, { 0.9f, 0.9f }, m_Game->m_BlackishColor);
				break;
			default:
				//Can::Renderer2D::DrawQuad({ x + offset.x, y + offset.y , 0.1f }, { 0.9f, 0.9f }, { 1.0f ,1.0f ,1.0f ,1.0f });
				break;
			}
		}
	}
}

glm::vec2 Play::GetNewApplePosition()
{
	int x = std::rand() % 12;
	int y = std::rand() % 12;
	while (m_State[y][x] != 0.01f)
	{
		x = std::rand() % 12;
		y = std::rand() % 12;
	}
	m_State[y][x] = 1;
	return glm::vec2(x, y);
}
