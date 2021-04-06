#include "Brain.h"

Brain::Brain():
	m_Reinforcementscore(0),
	m_PastReinforcementscore(10000)
{
	init();
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
		BPweightsLayer1.push_back(layer);
		RweightsLayer1.push_back(layer);
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
		BPweightsLayer2.push_back(layer);
		RweightsLayer2.push_back(layer);
		m_soft.push_back(0.0f);
		layer.clear();
	}

	//init vector arrays
	for (int i = 0; i < numOutputs; i++)
	{
		BPoutputs.push_back(make_shared<float>(0.0f));
		Routputs.push_back(make_shared<float>(0.0f)); 
		q_values.push_back(make_shared<float>(0.0f));
	}
	max_q = make_shared<float>(0.0f);
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
		BPoutputs = backprop(t_inputs);
	}
	return make_pair(BPoutputs, adjust);
}

void Brain::adjustWeights(vector<shared_ptr<float>> t_outputs)
{
	if (m_PastReinforcementscore < m_Reinforcementscore)
	{
		for (int i = 0; i < numHidden; i++)
		{
			for (int o = 0; o < numOutputs; o++)
			{
				//if (*t_outputs[o] == 1.0f)
				{
					// update weight
					RweightsLayer2[i][o] = make_shared<float>(*RweightsLayer2[i][o] + (m_learningRate * (*q_values[o] - m_soft[o]) * *t_outputs[o]));
				}
			}
		}
	}
}

float Brain::Sigmoid(float z)
{
	return 1.0 / (1.0 + exp(-z));
}

pair<vector<shared_ptr<float>>, bool> Brain::reinforcement(vector<shared_ptr<float>> t_inputs)
{
	m_PastReinforcementscore = m_Reinforcementscore;

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
			product = *t_inputs[j] * *RweightsLayer1[j][i];
			dot[i] += product;
		}
		m_soft[i] = ReLu(dot[i]);
	}
	SoftMaxVector(m_soft, numHidden);

	//apply weights hidden to output
	for (int i = 0; i < numOutputs; i++)
	{
		product = 0.0;
		for (int j = 0; j < numHidden; j++)
		{
			product = m_soft[j] * *RweightsLayer2[j][i];
			result += product;
		}
		result = Sigmoid(result);
		temp.push_back(make_shared<float>(result));
		result = 0.0f;
	}

	if (temp[4] < temp[0] && temp[4] < temp[1] &&
		temp[4] < temp[2] && temp[4] < temp[3])
	{
		if (temp[0] > temp[1]) // left or right
		{
			temp[0] = make_shared<float>(1.0f);
			temp[1] = make_shared<float>(0.0f);
		}
		else
		{
			temp[1] = make_shared<float>(1.0f);
			temp[0] = make_shared<float>(0.0f);
		}
		if (temp[2] > temp[3]) // accel or decel
		{
			temp[2] = make_shared<float>(1.0f);
			temp[3] = make_shared<float>(0.0f);
		}
		else
		{
			temp[2] = make_shared<float>(1.0f);
			temp[3] = make_shared<float>(0.0f);
		}
	}
	updateQValues();

	pair<vector<shared_ptr<float>>, bool> p = make_pair(temp, false);

	return p;
}

void Brain::updateQValues()
{
	for (int i = 0; i < q_values.size(); i++)
	{
		q_values[i] = make_shared<float>((*q_values[i] * (1.0f - m_learningRate)) + (m_learningRate * (m_Reinforcementscore + (m_discountFactor * *max_q))));
	}

	max_q = make_shared<float>(0.0f);

	for (int i = 0; i < q_values.size(); i++)
	{
		if (q_values[i] > max_q)
			max_q = q_values[i];
	}
}

void Brain::saveRWeights()
{
}

/// <summary>
/// load backprop weights
/// </summary>
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
				BPweightsLayer1[i][index] = make_shared<float>(weight);
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < 5; i++)
		{
			getline(weightsFile, line);
			//ignore biases
		}
		// layer 2
		for (int i = 0; i < 5; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				BPweightsLayer2[i][index] = make_shared<float>(weight);
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < 5; i++)
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
			product = *t_inputs[j] * *BPweightsLayer1[j][i];
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
			product = soft[j] * *BPweightsLayer2[j][i];
			result += product;
		}
		result = Sigmoid(result);
		temp.push_back(make_shared<float>(result));
		result = 0.0f;
	}

	if(temp[4] < temp[0] && temp[4] < temp[1] &&
		temp[4] < temp[2] && temp[4] < temp[3])
	{
		if (temp[0] > temp[1]) // left or right
		{
			temp[0] = make_shared<float>(1.0f);
			temp[1] = make_shared<float>(0.0f);
		}
		else
		{
			temp[1] = make_shared<float>(1.0f);
			temp[0] = make_shared<float>(0.0f);
		}
		if (temp[2] > temp[3]) // accel or decel
		{
			temp[2] = make_shared<float>(1.0f);
			temp[3] = make_shared<float>(0.0f);
		}
		else
		{
			temp[2] = make_shared<float>(1.0f);
			temp[3] = make_shared<float>(0.0f);
		}
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

void Brain::SoftMaxVector(vector<float> data, int len)
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
