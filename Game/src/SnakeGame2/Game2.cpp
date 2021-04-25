#include "canpch.h"
#include "Game2.h"
#include <math.h> 

Game2::Game2(unsigned int playCount)
	: m_StartHealth(100)
	, m_InputSize(30)
	, m_Dimension(30)
	, m_PlayCount(playCount)
	, m_PlayLeft(playCount)
{
	m_Plays = new Play2 * [m_PlayCount];
	for (unsigned int i = 0; i < m_PlayCount; i++)
		m_Plays[i] = new Play2(this, m_Directions[0], m_StartHealth, new NeuralNetwork(GetNodes(), 4, 0.05f), m_Dimension, m_InputSize);
}

Game2::~Game2()
{
	for (unsigned int i = 0; i < m_PlayCount; i++)
		delete m_Plays[i];

	delete[] m_Plays;
}

void Game2::DrawBestPlay()
{
	int* points = new int[m_PlayCount];

	for (unsigned int i = 0; i < m_PlayCount; i++)
		points[i] = m_Plays[i]->GetPoints();

	int* maxElement = std::max_element(points, points + m_PlayCount);
	_int64 index = std::distance(points, maxElement);

	m_Plays[index]->Draw(m_Offset);

	delete[] points;
}

void Game2::Tick(float ts)
{
	static unsigned int generation = 0;
	if (m_PlayLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_PlayCount];
		int* points = new int[m_PlayCount];
		int total = 0;
		for (unsigned int i = 0; i < m_PlayCount; i++)
		{
			brains[i] = m_Plays[i]->GetBrain();
			points[i] = m_Plays[i]->GetPoints();
			total += points[i];
		}

		int* maxElement = std::max_element(points, points + m_PlayCount);
		_int64 index = std::distance(points, maxElement);
		m_GenerationData.push_back(points[index]);

		brains = NeuralNetwork::Generate(brains, points, total, m_PlayCount, m_PlayCount);
		brains = NeuralNetwork::Mutate(brains, 1, m_PlayCount);
		m_PlayLeft = m_PlayCount;

		for (unsigned int i = 0; i < m_PlayCount; i++)
			delete m_Plays[i];


		for (unsigned int i = 0; i < m_PlayCount; i++)
		{
			m_Plays[i] = new Play2(this, m_Directions[0], m_StartHealth, brains[i], m_Dimension,m_InputSize);
		}
		delete[] points;
	}
	else
	{
		for (unsigned int i = 0; i < m_PlayCount; i++)
		{
			if (m_Plays[i]->IsDeath())
				continue;
			m_Plays[i]->Tick();
		}
	}
}
