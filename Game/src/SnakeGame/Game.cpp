#include "canpch.h"
#include "Game.h"

Game::Game(unsigned int playCount)
	: m_PlayCount(playCount)
	, m_PlayLeft(playCount)
{
	m_Plays = new Play * [m_PlayCount];
	for (unsigned int i = 0; i < m_PlayCount; i++)
	{
		m_Plays[i] = new Play(this, m_Directions[0], 10);
	}
}

Game::~Game()
{
	for (unsigned int i = 0; i < m_PlayCount; i++)
		delete m_Plays[i];

	delete[] m_Plays;
}

void Game::DrawBestPlay()
{
	int* points = new int[m_PlayCount];

	for (unsigned int i = 0; i < m_PlayCount; i++)
		points[i] = m_Plays[i]->GetPoints();

	int* maxElement = std::max_element(points, points + m_PlayCount);
	_int64 index = std::distance(points, maxElement);

	m_Plays[index]->Draw(m_Offset);

	delete[] points;
	//delete maxElement;
}

void Game::DrawThat(int index)
{
	m_Plays[index]->Draw(m_Offset2);
}

void Game::Tick(float ts)
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
		for (unsigned int i = 0; i < m_PlayCount; i++)
		{
			points[i] /= total;
			points[i] *= 100.0f;
		}

		float* p = new float[m_PlayCount];

		for (unsigned int i = 0; i < m_PlayCount; i++)
			p[i] = m_Plays[i]->GetPoints();

		brains = NeuralNetwork::Generate(brains, points, total, m_PlayCount, m_PlayCount);
		brains = NeuralNetwork::Mutate(brains, 5, m_PlayCount);
		m_PlayLeft = m_PlayCount;

		for (int i = 0; i < m_PlayCount; i++)
			delete m_Plays[i];


		for (int i = 0; i < m_PlayCount; i++)
		{
			m_Plays[i] = new Play(this, m_Directions[0], 10, brains[i]);
		}
		delete[] points;

		float* maxElement = std::max_element(p, p + m_PlayCount);
		int index = std::distance(p, maxElement);
		m_GenerationData.push_back(p[index]);

		delete[] p;
	}
	else
	{
		
		static float time = 0.0f;
		time += ts;
		if (time > 0.1f)
		{
			time = 0.0f;
			for (int i = 0; i < m_PlayCount; i++)
			{
				if (m_Plays[i]->IsDeath())
					continue;
				m_Plays[i]->Tick();
			}
		}
	}
}
