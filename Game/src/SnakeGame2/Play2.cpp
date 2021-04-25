#include "canpch.h"
#include "Play2.h"
#include "Game2.h"

Play2::Play2(Game2* game, glm::vec2 direction, int health, NeuralNetwork* brain, int dimension, int inputSize)
	: m_CurrentDirection(direction)
	, m_Health(health)
	, m_Game(game)
	, m_Brain(brain)
	, m_Dimension(dimension)
	, m_InputSize(inputSize)
{
	m_State = new int* [m_Dimension];
	for (int i = 0; i < m_Dimension; i++)
	{
		m_State[i] = new int[m_Dimension];
	}
	for (int i = 0; i < m_Dimension; i++)
	{
		for (int j = 0; j < m_Dimension; j++)
		{
			m_State[i][j] = 0;
		}
	}

	int x = m_Dimension / 2;
	int y = x;

	m_Snake.push_back({ x, y - 1 });
	m_Snake.push_back({ x, y });

	m_State[x][y - 1] = -1;
	m_State[x][y] = -1;

	m_ApplePosition = GetNewApplePosition();
}

Play2::~Play2()
{
	delete m_Brain;
	for (int i = 0; i < m_Dimension; i++)
		delete m_State[i];
	delete[] m_State;
}

void Play2::Tick()
{

	float* state = new float[m_InputSize];

	glm::vec2 sb = m_Snake.back();
	for (int i = 0; i < 8; i++)
	{
		glm::vec2 direction = i % 2 ? m_Game->m_Directions[i / 2] : m_Game->m_Directions[i / 2] + m_Game->m_Directions[(i / 2) + 1];
		{
			int dist = 1;
			glm::vec2 pos = { sb.x,sb.y };
			pos += direction;
			while (true)
			{
				if (pos.x >= m_Dimension || pos.y >= m_Dimension || pos.x < 0 || pos.y < 0)
				{
					dist = 0;
					break;
				}
				if (m_State[(int)pos.x][(int)pos.y] == 1)
					break;
				pos += direction;
			}
			state[i * 3 + 0] = (float)dist;
		}
		{
			int dist = 1;
			glm::vec2 pos = { sb.x,sb.y };
			pos += direction;
			while (true)
			{
				if (pos.x >= m_Dimension || pos.y >= m_Dimension || pos.x < 0 || pos.y < 0)
				{
					break;
				}
				pos += direction;
				dist++;
			}
			state[i * 3 + 1] = (float)(dist) / m_Dimension;
		}
		{
			int dist = 1;
			glm::vec2 pos = { sb.x,sb.y };
			pos += direction;
			while (true)
			{
				if (pos.x >= m_Dimension || pos.y >= m_Dimension || pos.x < 0 || pos.y < 0)
				{
					dist = 0;
					break;
				}
				if (m_State[(int)pos.x][(int)pos.y] == -1)
					break;
				pos += direction;
			}
			state[i * 3 + 2] = (float)dist;
		}
	}
	state[m_InputSize - 6] = m_CurrentDirection.x;
	state[m_InputSize - 5] = m_CurrentDirection.y;
	state[m_InputSize - 4] = sb.x > m_ApplePosition.x ? 1 : 0;
	state[m_InputSize - 3] = sb.y > m_ApplePosition.y ? 1 : 0;
	state[m_InputSize - 2] = sb.x < m_ApplePosition.x ? 1 : 0;
	state[m_InputSize - 1] = sb.y < m_ApplePosition.y ? 1 : 0;
	Matrix* input = new Matrix(m_InputSize, 1, state);
	Matrix* result = m_Brain->FeedForward(input);

	float A[3] = {
		result->data[0],
		result->data[1],
		result->data[2]
	};
	float* maxElement = std::max_element(A, A + 3);
	_int64 dist = std::distance(A, maxElement);
	m_DirectionIndex = (m_DirectionIndex + ((dist == 0) ? 1 : dist == 1 ? 0.0f : -1)) + 4 % 4;
	m_CurrentDirection = m_Game->m_Directions[m_DirectionIndex];
	delete result;

	b_IsDeath = Update();
	if (b_IsDeath)
		m_Game->PlayerDeath();

	delete[] state;
}

bool Play2::Update()
{
	m_Health--;
	//m_Points++;

	glm::vec2 position = m_Snake.back();
	glm::vec2 nextPos = position + m_CurrentDirection;

	nextPos.x = (int)(nextPos.x + m_Dimension) % m_Dimension;
	nextPos.y = (int)(nextPos.y + m_Dimension) % m_Dimension;

	glm::vec2 tailPos = m_Snake.front();
	m_State[(int)(tailPos.x)][(int)(tailPos.y)] = 0;

	if (m_State[(int)(nextPos.x)][(int)(nextPos.y)] == -1)
	{
		return true;
	}
	if (m_State[(int)(nextPos.x)][(int)(nextPos.y)] == 1)
	{
		m_Snake.insert(m_Snake.begin(), m_Snake.front());
		m_Health = m_Game->m_StartHealth;
		m_Points += 20;
		m_ApplePosition = GetNewApplePosition();
	}
	if (m_Health <= 0)
		return true;
	m_Snake.push_back(nextPos);
	m_State[(int)(nextPos.x)][(int)(nextPos.y)] = -1;
	m_Snake.erase(m_Snake.begin());
	return false;
}

void Play2::Draw(glm::vec2 offset)
{
	Can::DrawQuadParameters params = Can::DrawQuadParameters();
	params.Size = { 0.9f, 0.9f };
	int half = m_Dimension / 2;
	for (int x = -half; x < half; x++)
	{
		for (int y = -half; y < half; y++)
		{
			params.Position = { x + offset.x, y + offset.y , 0.1f };
			switch ((int)m_State[x + half][y + half])
			{
			case 1:
				params.TintColor = m_Game->m_RedColor;
				Can::Renderer2D::DrawQuad(params);
				break;
			case -1:
				params.TintColor = m_Game->m_BlackishColor;
				Can::Renderer2D::DrawQuad(params);
				break;
			default:
				params.TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				Can::Renderer2D::DrawQuad(params);
				break;
			}
		}
	}

	params.Position = { m_Snake.back().x - half + offset.x, m_Snake.back().y - half + offset.y , 0.2f };
	params.Size = glm::vec2(1.0f);
	params.TintColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	Can::Renderer2D::DrawQuad(params );
}

glm::vec2 Play2::GetNewApplePosition()
{
	int x = std::rand() % m_Dimension;
	int y = std::rand() % m_Dimension;
	while (m_State[x][y] != 0)
	{
		x = std::rand() % m_Dimension;
		y = std::rand() % m_Dimension;
	}
	m_State[x][y] = 1;
	return glm::vec2(x, y);
}
