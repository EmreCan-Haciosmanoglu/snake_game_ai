#include "NeuralNetwork.h"
#include "canpch.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#define SIGMOID (func)([](float x) {return 1.0f / (1.0f + exp(-x));})
#define D_SIGMOID (func)([](float y) {return y*(1.0f-y);})

NeuralNetwork::NeuralNetwork(int* nodes, int length, float lr)
	: Nodes(nodes)
	, Length(length)
	, learning_rate(lr)
{
	weights = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		weights[i] = new Matrix(nodes[i + 1], nodes[i]);
		weights[i]->Randomize(-2.0f, 2.0f);
	}
	biases = new Matrix * [length - 1];
	for (int i = 0; i < length - 1; i++)
	{
		biases[i] = new Matrix(nodes[i + 1], 1);
		biases[i]->Randomize(-2.0f, 2.0f);
	}
}
NeuralNetwork::NeuralNetwork(int* nodes, int length, float lr, Matrix * *w, Matrix * *b)
	: Nodes(nodes)
	, Length(length)
	, learning_rate(lr)
{
	weights = w;
	biases = b;
}

NeuralNetwork::~NeuralNetwork()
{
	delete Nodes;
	for (int i = 0; i < Length - 1; i++)
		delete weights[i];
	delete[] weights;
	for (int i = 0; i < Length - 1; i++)
		delete biases[i];
	delete[] biases;
}

Matrix* NeuralNetwork::FeedForward(Matrix * inputs) const
{
	Matrix** layerOutputs = new Matrix * [Length];
	layerOutputs[0] = inputs;
	for (int i = 1; i < Length; i++)
	{
		layerOutputs[i] = Matrix::MatrixMultiplication(weights[i - 1], layerOutputs[i - 1]);
		*layerOutputs[i] += *biases[i - 1];
		layerOutputs[i]->Activation(SIGMOID);
	}
	Matrix * result = new Matrix(*layerOutputs[Length - 1]);
	for (int i = 0; i < Length; i++)
	{
		delete layerOutputs[i];
	}
	delete[] layerOutputs;
	return result;
}

void NeuralNetwork::FeedForward(Matrix * *outputs) const
{
	for (int i = 1; i < Length; i++)
	{
		outputs[i] = Matrix::MatrixMultiplication(weights[i - 1], outputs[i - 1]);
		*outputs[i] += *biases[i - 1];
		outputs[i]->Activation(SIGMOID);
	}
}

void NeuralNetwork::Train(Matrix * inputs, Matrix * targets)
{
	Matrix** outputs = new Matrix * [Length];
	outputs[0] = inputs;
	FeedForward(outputs);

	Matrix** errors = new Matrix * [Length];
	errors[Length - 1] = *targets - *outputs[Length - 1];
	delete targets;

	for (int i = Length - 2; i >= 0; i--)
	{
		Matrix* T = Matrix::Transpose(weights[i]);
		errors[i] = Matrix::MatrixMultiplication(T, errors[i + 1]);
		delete T;
	}

	for (int i = Length - 2; i >= 0; i--)
	{
		Matrix* gradients = Matrix::Map(outputs[i + 1], D_SIGMOID);

		*gradients *= *errors[i + 1];
		*gradients *= learning_rate;
		Matrix * T = Matrix::Transpose(outputs[i]);
		Matrix * deltas = Matrix::MatrixMultiplication(gradients, T);

		*weights[i] += *deltas;
		*biases[i] += *gradients;

		delete gradients;
		delete T;
		delete deltas;
	}
	for (int i = 0; i < Length; i++)
	{
		delete outputs[i];
		delete errors[i];
	}
	delete[] outputs;
	delete[] errors;
}

NeuralNetwork** NeuralNetwork::Generate(NeuralNetwork * *parents, int* fitnessScores, int totalScore, int parentCount, int childrenCount)
{
	NeuralNetwork** children = new NeuralNetwork * [childrenCount];
	totalScore *= totalScore;

	auto rng = std::default_random_engine{};
	for (int j = 0; j < childrenCount; j++)
	{
		NeuralNetwork* parentA = nullptr;
		{
			int guess = std::rand() % totalScore;
			int line = 0;
			for (int i = 0; i < parentCount; i++)
			{
				line += fitnessScores[i]* fitnessScores[i];
				if (guess < line || i == parentCount - 1)
				{
					parentA = parents[i];
					break;
				}
			}
		}
		NeuralNetwork* parentB = nullptr;
		{
			int guess = std::rand() % totalScore;
			int line = 0;
			for (int i = 0; i < parentCount; i++)
			{
				line += fitnessScores[i]* fitnessScores[i];
				if (guess < line || i == parentCount - 1)
				{
					parentB = parents[i];
					break;
				}
			}
		}
		int len = parentA->GetLength();
		int* nodes = new int[len];
		int* parentNodes = parentA->GetNodes();
		for (int k = 0; k < len; k++)
		{
			nodes[k] = parentNodes[k];
		}

		int size = 0;

		Matrix** weights = new Matrix * [len - 1];
		for (int i = 0; i < len - 1; i++)
		{
			size += nodes[i + 1] * nodes[i];
			weights[i] = new Matrix(nodes[i + 1], nodes[i]);
		}

		Matrix * *biases = new Matrix * [len - 1];
		for (int i = 0; i < len - 1; i++)
		{
			size += nodes[i + 1];
			biases[i] = new Matrix(nodes[i + 1], 1);
		}

		std::vector<int> indexes;

		{
			int half = size / 2;
			for (int i = 0; i < half; i++)
				indexes.push_back(1);
			for (int i = half; i < size; i++)
				indexes.push_back(0);

			std::shuffle(std::begin(indexes), std::end(indexes), rng);
		}

		{
			int index1 = 0;
			for (int i = 0; i < len - 1; i++)
			{
				int index2 = 0;
				int leng = nodes[i + 1] * nodes[i];
				float* dataA = parentA->GetWeights()[i]->data;
				float* dataB = parentB->GetWeights()[i]->data;
				float* dataC = weights[i]->data;
				for (int f = 0; f < leng; f++)
				{
					if (indexes[index1])
						dataC[f] = dataA[f];
					else
						dataC[f] = dataB[f];
					index1++;
					index2++;
				}
			}
			for (int i = 0; i < len - 1; i++)
			{
				int index2 = 0;
				int leng = nodes[i + 1];
				float* dataA = parentA->GetBiases()[i]->data;
				float* dataB = parentB->GetBiases()[i]->data;
				float* dataC = biases[i]->data;
				for (int f = 0; f < leng; f++)
				{
					if (indexes[index1])
						dataC[f] = dataA[f];
					else
						dataC[f] = dataB[f];
					index1++;
					index2++;
				}
			}
		}
		indexes.clear();
		children[j] = new NeuralNetwork(nodes, len, parentA->GetLR(), weights, biases);
	}

	return children;
}

NeuralNetwork** NeuralNetwork::Mutate(NeuralNetwork * *source, int mutationRate, int sourceCount)
{
	for (int i = 0; i < sourceCount; i++)
	{
		NeuralNetwork* brain = source[i];
		int* nodes = brain->GetNodes();
		int len = brain->GetLength();
		Matrix** weights = brain->GetWeights();
		Matrix** biases = brain->GetBiases();


		for (int j = 0; j < len - 1; j++)
		{
			int leng = nodes[j + 1] * nodes[j];
			float* data = weights[j]->data;
			for (int k = 0; k < leng; k++)
			{
				int guess = std::rand() % 100;
				if (guess < mutationRate)
					data[k] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 4.0f - 2.0f;

			}
		}

		for (int j = 0; j < len - 1; j++)
		{
			int leng = nodes[j + 1];
			float* data = biases[j]->data;
			for (int k = 0; k < leng; k++)
			{
				int guess = std::rand() % 100;
				if (guess < mutationRate)
					data[k] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 4.0f - 2.0f;

			}
		}
	}

	return source;
}

void NeuralNetwork::SetWeights(Matrix * *w)
{
	if (weights != nullptr)
	{
		for (int j = 0; j < Length - 1; j++)
		{
			if (weights[j] != nullptr)
				delete weights[j];
		}

		delete[] weights;
	}
	weights = w;
}

void NeuralNetwork::SetBiases(Matrix * *b)
{
	if (biases != nullptr)
	{
		for (int j = 0; j < Length - 1; j++)
		{
			if (biases[j] != nullptr)
				delete biases[j];
		}

		delete[] biases;
	}
	biases = b;
}
