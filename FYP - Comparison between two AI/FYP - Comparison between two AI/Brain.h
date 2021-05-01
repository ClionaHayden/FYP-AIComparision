#pragma once

#include <time.h>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>

#include "GameState.h"

using namespace std;

class Brain
{
public:
	const int numInputs = 13;
	const int numHidden = 1;
	const int numOutputs = 5;

	vector<vector<float>>  BPweightsLayer1;	// From inputs to hidden layer
	vector<vector<float>>   BPweightsLayer2;	// From hidden layer to output neuron
	float BPbiases;
	vector<float> BPOutputs;


	vector<vector<float>> RweightsLayer1;	// From inputs to hidden layer
	vector<vector<float>> RweightsLayer2;	// From hidden layer to output neuron
	vector<float> Routputs;
	float Rbiases;

	vector<float> q_values;
	float max_q;
	vector<float> m_soft;
	float m_learningRate = 0.5f;
	float m_phi = 0.0f;
	float m_discountRate = 0.05f;
	float m_discountFactor = 1.0f / (1.0f + m_discountRate);

	bool useBiases = false;
	float m_Reinforcementscore;
	float m_PastReinforcementscore;
	float m_bestRScore;

	std::ifstream weightsFile;

	Brain();
	~Brain();
	void init();

	pair<vector<float>,bool> Evaluate(vector<float> t_inputs,bool t_reinforcement);
	void adjustWeights(vector<float> t_outputs);
	float Sigmoid(float z);

private:
	pair<vector<float>, bool> reinforcement(vector<float> t_inputs);
	void saveRWeights();
	void loadBPWeights();
	void loadRWeights();
	vector<float> backprop(vector<float> t_inputs);

	void SoftMax(float data[], int len);
	void SoftMaxVector(vector<float> data, int len);
	float ReLu(float val);
};
