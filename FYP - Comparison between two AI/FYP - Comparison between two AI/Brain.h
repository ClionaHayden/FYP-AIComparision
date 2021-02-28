#pragma once

#include <time.h>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace std;

class Brain
{
public:
	const int numInputs = 5;
	const int numHidden = 5;
	const int numOutputs = 5;

	vector<vector<shared_ptr<float>>> weightsLayer1;	// From inputs to hidden layer
	vector<vector<shared_ptr<float>>>  weightsLayer2;	// From hidden layer to output neuron
	vector<shared_ptr<float>> outputs;
	shared_ptr<float> biases;

	float m_learningRate = 0.5;
	float m_forgetRate = 0.05;

	bool useBiases = false;
	int m_Reinforcementscore;
	int m_PastReinforcementscore;

	Brain();
	~Brain();
	void init();

	pair<vector<shared_ptr<float>>,bool> Evaluate(vector<shared_ptr<float>> t_inputs);
	void adjustWeights(vector<shared_ptr<float>> t_inputs, vector<shared_ptr<float>> t_hidden);
	float Sigmoid(float z);
};
