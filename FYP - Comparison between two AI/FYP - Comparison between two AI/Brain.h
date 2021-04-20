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
	const int numInputs = 12;
	const int numHidden = 40;
	const int numOutputs = 5;

	vector<vector<shared_ptr<float>>> BPweightsLayer1;	// From inputs to hidden layer
	vector<vector<shared_ptr<float>>>  BPweightsLayer2;	// From hidden layer to output neuron
	vector<shared_ptr<float>> BPoutputs;
	shared_ptr<float> BPbiases;
	vector<shared_ptr<float>> BPHiddenOutputs;


	vector<vector<shared_ptr<float>>> RweightsLayer1;	// From inputs to hidden layer
	vector<vector<shared_ptr<float>>> RweightsLayer2;	// From hidden layer to output neuron
	vector<shared_ptr<float>> Routputs;
	shared_ptr<float> Rbiases;

	vector<shared_ptr<float>> q_values;
	shared_ptr<float> max_q;
	vector<float> m_soft;
	float m_learningRate = 0.5f;
	float m_phi = 0.0f;
	float m_discountRate = 0.05f;
	float m_discountFactor = 1.0f / (1.0f + m_discountRate);

	bool useBiases = false;
	int m_Reinforcementscore;
	int m_PastReinforcementscore;

	std::ifstream weightsFile;

	Brain();
	~Brain();
	void init();

	pair<vector<shared_ptr<float>>,bool> Evaluate(vector<shared_ptr<float>> t_inputs,bool t_reinforcement);
	void adjustWeights(vector<shared_ptr<float>> t_outputs);
	float Sigmoid(float z);

private:
	pair<vector<shared_ptr<float>>, bool> reinforcement(vector<shared_ptr<float>> t_inputs);
	void saveRWeights();
	void loadBPWeights();
	void loadRWeights();
	vector<shared_ptr<float>> backprop(vector<shared_ptr<float>> t_inputs);

	void SoftMax(float data[], int len);
	void SoftMaxVector(vector<float> data, int len);
	float ReLu(float val);
};
