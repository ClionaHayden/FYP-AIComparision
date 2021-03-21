#include "Brain.h"

Brain::Brain():
	m_Reinforcementscore(0),
	m_PastReinforcementscore(0)
{
	init();

	vector<shared_ptr<float>> inputs;
	for (int i = 0; i < numInputs; i++)
	{
		inputs.push_back(make_shared<float>(0.5));
	}
	//Evaluate(inputs);
}

Brain::~Brain()
{
}

void Brain::init()
{
	//set weights to random values between -1 and 1
	//input layer to hidden
	vector<shared_ptr<float>> layer;
	for (int i = 0; i < numInputs; i++)
	{
		for (int a = 0; a < numHidden; a++)
		{
			float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))) - 1;
			layer.push_back(make_shared<float>(r));
		}
		weightsLayer1.push_back(layer);
		layer.clear();
	}

	// hidden to output
	for (int i = 0; i < numHidden; i++)
	{
		for (int a = 0; a < numOutputs; a++)
		{
			float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))) - 1;
			layer.push_back(make_shared<float>(r));
		}
		weightsLayer2.push_back(layer);
		layer.clear();
	}

	//init vector arrays
	for (int i = 0; i < numOutputs; i++)
	{
		outputs.push_back(make_shared<float>(0.0f));
	}
}

pair<vector<shared_ptr<float>>, bool> Brain::Evaluate(vector<shared_ptr<float>> t_inputs)
{
	bool adjust = false;
	if (s_gameState == GameState::Reinforcement)
	{
		return reinforcement(t_inputs);
	}
	else if (s_gameState == GameState::LoadWeights)
	{
		loadWeights();
		backprop(t_inputs);
	}
	return make_pair(outputs, adjust);
}

void Brain::adjustWeights(vector<shared_ptr<float>> t_inputs, vector<shared_ptr<float>> t_hidden)
{
	// apply hebb's update rule
	// input to hidden
	for (int i = 0; i < numInputs; i++)
	{
		for (int a = 0; a < numHidden; a++)
		{
			weightsLayer1[i][a] = make_shared<float>(m_learningRate * *t_inputs[i] * *t_hidden[a]);
			//less forgetting factor
			weightsLayer1[i][a] = make_shared<float>(*weightsLayer1[i][a] - (m_forgetRate * *t_inputs[i] * *weightsLayer1[i][a]));
		}
	}

	// hidden to output
	for (int i = 0; i < numHidden; i++)
	{
		for (int a = 0; a < numOutputs; a++)
		{
			weightsLayer2[i][a] = make_shared<float>(m_learningRate * *t_hidden[i] * *outputs[a]);
			//less forgetting factor
			weightsLayer2[i][a] = make_shared<float>(*weightsLayer2[i][a] - (m_forgetRate * *t_hidden[i] * *weightsLayer2[i][a]));
		}
	}
}

float Brain::Sigmoid(float z)
{
	return 1.0 / (1.0 + exp(-z));
}

pair<vector<shared_ptr<float>>, bool> Brain::reinforcement(vector<shared_ptr<float>> t_inputs)
{
	bool adjust = false;
	// feed forward

		// input to hidden 
	vector<shared_ptr<float>> hiddenVals;
	for (int a = 0; a < numHidden; a++)
	{
		hiddenVals.push_back(make_shared<float>(0));
	}

	for (int i = 0; i < numInputs; i++)
	{
		for (int a = 0; a < numHidden; a++)
		{
			hiddenVals[a] = (make_shared<float>(*hiddenVals[a] + (*t_inputs[a] * *weightsLayer1[i][a])));
			//hiddenVals[a] = make_shared<float>(Sigmoid(*hiddenVals[a]));
			//if (*hiddenVals[a] > 0.5)
			//	hiddenVals[a] = make_shared<float>(1);
			//else
			//	hiddenVals[a] = make_shared<float>(0);
		}
	}

	// hidden to output
	for (int i = 0; i < numHidden; i++)
	{
		for (int a = 0; a < numOutputs; a++)
		{
			outputs[a] = make_shared<float>(*outputs[a] + (*hiddenVals[i] * *weightsLayer2[i][a]));
		}
	}

	//apply sigmoid
	for (int i = 0; i < numOutputs; i++)
	{
		outputs[i] = make_shared<float>(Sigmoid(*outputs[i]));
		if (*outputs[i] > 0.5)
			outputs[i] = make_shared<float>(1);
		else
			outputs[i] = make_shared<float>(0);
	}
	if (m_Reinforcementscore < m_PastReinforcementscore)
	{
		adjust = true;
		adjustWeights(t_inputs, hiddenVals);
	}
	return make_pair(outputs, adjust);
}

void Brain::loadWeights()
{
	vector<string> row;
	string line, word, temp;
	int layer = 0;
	int index = 0;
	float weight = 0.0f;

	weightsFile.open("DATA/CalculatedWeights.csv");

	while (!weightsFile.eof())
	{
		//layer 1
		for (int i = 0; i < 10; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				weightsLayer1[i][index] = make_shared<float>(weight);
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			getline(weightsFile, line);
			//ignore biases
		}
		// layer 2
		for (int i = 0; i < 4; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				weightsLayer2[i][index] = make_shared<float>(weight);
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			getline(weightsFile, line);
			//ignore bias
		}
	}
	weightsFile.close();
}

vector<shared_ptr<float>> Brain::backprop(vector<shared_ptr<float>> t_inputs)
{
	vector<shared_ptr<float>> temp;
	float result = 0.0;
	int layer = 0;

	float dot[5] = { 0.0 };
	float soft[5] = { 0.0 };
	float product = 0.0;

	// apply weights input to hidden layer
	for (int i = 0; i < numHidden; i++)
	{
		product = 0.0;
		for (int j = 0; j < numInputs; j++)
		{
			product = *t_inputs[j] * *weightsLayer1[j][i];
			dot[i] += product;
		}
		soft[i] = ReLu(dot[i]);
	}
	SoftMax(soft, numHidden);

	//apply weights hidden to output
	for (int i = 0; i < numOutputs; i++)
	{
		product = 0.0;
		for (int j = 0; j < numHidden; j++)
		{
			product = soft[j] * *weightsLayer2[j][i];
			result += product;
		}
		result = Sigmoid(result);
		if (result > 0.5)
			result = 1;
		else
			result = 0;
		temp.push_back(make_shared<float>(result));
	}

	return temp;
}

void Brain::SoftMax(float data[], int len)
{
	float sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum = sum + data[i];
	}
	if (sum == 0) sum = 1;
	for (int i = 0; i < len; i++)
	{
		data[i] = data[i] / sum;
	}
}

float Brain::ReLu(float val)
{
	if (val < 0) val = 0;
	return val;
}
