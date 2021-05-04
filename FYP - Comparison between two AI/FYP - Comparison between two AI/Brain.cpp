#include "Brain.h"

Brain::Brain():
	m_Reinforcementscore(-10000),
	m_PastReinforcementscore(-10000),
	m_bestRScore(-10000)
{
	init();
}

Brain::~Brain()
{
}
/// <summary>
/// initialise brain to random weights before it is trained
/// </summary>
void Brain::init()
{
	//set weights to random values between -1 and 1
	//input layer to hidden
	vector<float> layer;
	for (int i = 0; i < numInputs; i++)
	{
		for (int a = 0; a < numHidden; a++)
		{
			float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))) - 1;
			layer.push_back(r);
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
			layer.push_back(r);
		}
		BPweightsLayer2.push_back(layer);
		RweightsLayer2.push_back(layer);
		m_soft.push_back(0.0f);
		layer.clear();
	}

	//init vector arrays
	for (int i = 0; i < numOutputs; i++)
	{
		BPOutputs.push_back((0.0f));
		Routputs.push_back((0.0f)); 
		q_values.push_back((0.0f));
	}
	max_q = 0.0f;
}

/// <summary>
/// Calculates the brain's outputs 
/// </summary>
/// <param name="t_inputs"> inputs for the brain to process </param>
/// <param name="t_reinforcement"> true if the brain is to run from reinforcement weights </param>
/// <returns> new outputs </returns>
pair<vector<float>, bool> Brain::Evaluate(vector<float> t_inputs, bool t_reinforcement)
{
	bool adjust = false;
	if (s_gameState == GameState::Reinforcement)
	{
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			cout << "Saving Data...." << endl;
			saveRWeights();
			cout << "Saved" << endl;
		}
		return reinforcement(t_inputs);
	}
	else if (s_gameState == GameState::LoadWeights)
	{
		if (t_reinforcement)
		{
			loadRWeights();
			return reinforcement(t_inputs);
		}
		else
		{
			loadBPWeights();
			BPOutputs = backprop(t_inputs);
		}
	}
	return make_pair(BPOutputs, adjust);
}

/// <summary>
/// Adjusts the reinforcemnet  weights as per the Q-Learning algorithm, currently inaccurate
/// </summary>
/// <param name="t_outputs"> brain outputs </param>
void Brain::adjustWeights(vector<float> t_outputs)
{
	if (m_PastReinforcementscore > m_Reinforcementscore)
	{
		if (m_Reinforcementscore > m_bestRScore)
		{
			saveRWeights();
			m_bestRScore = m_Reinforcementscore;
		}
		else
		{
			loadRWeights();
		}
		float maxQ = 0;
		vector<float> newQValues;
		m_phi = 0;
		for (int i = 0; i < numOutputs; i++)
		{
			if (q_values[i] > maxQ)
			{
				maxQ = q_values[i];
			}
		}
		for (int i = 0; i < numOutputs; i++)
		{
			float newQ;
			newQ = (1 - m_learningRate) * q_values[i] + m_learningRate * (m_Reinforcementscore + m_discountFactor * maxQ);
			newQValues.push_back(newQ);
		}
		for (int i = 0; i < numHidden; i++)
		{
			for (int o = 0; o < numOutputs; o++)
			{
				//if (t_outputs[o] == 1.0f)
				{
					float error = newQValues[o] - q_values[o];
					m_phi += error;
					RweightsLayer2[i][o] = (RweightsLayer2[i][o] + (error * q_values[o]));
				}
			}
		}
		for (int i = 0; i < numInputs; i++)
		{
			for (int j = 0; j < numHidden; j++)
			{
				float error = m_soft[j] * (1 - m_soft[j]) * m_learningRate * m_phi;
				RweightsLayer1[i][j] = (RweightsLayer1[i][j] + (error * m_soft[j]));
			}
		}
	}
}

/// <summary>
/// the sigmoid activation function for the neural networks
/// </summary>
/// <param name="z"> value to be evaluated </param>
/// <returns> sigmoid value of z </returns>
float Brain::Sigmoid(float z)
{
	return 1.0 / (1.0 + exp(-z));
}

/// <summary>
/// Evaluates the brain based off the reinforcement weights
/// </summary>
/// <param name="t_inputs"> inputs to be processed </param>
/// <returns> calculated outputs </returns>
pair<vector<float>, bool> Brain::reinforcement(vector<float> t_inputs)
{
	m_PastReinforcementscore = m_Reinforcementscore;

	vector<float> temp;
	float result = 0.0;
	int layer = 0;

	float dot[1] = { 0.0 };
	float soft[1] = { 0.0 };
	float product = 0.0;

	// apply weights input to hidden layer
	for (int i = 0; i < numHidden; i++)
	{
		product = 0.0;
		for (int j = 0; j < numInputs; j++)
		{
			product = t_inputs[j] * RweightsLayer1[j][i];
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
			product = m_soft[j] * RweightsLayer2[j][i];
			result += product;
		}
		result = Sigmoid(result);
		temp.push_back(result);
		result = 0.0f;
	}
	q_values = temp;
	if ((temp[4] < temp[0]) && (temp[4] < temp[1]) &&
		(temp[4] < temp[2]) && (temp[4] < temp[3]))
	{
		if (q_values[0] > q_values[1]) // left or right
		{
			temp[0] = (1.0f);
			temp[1] = (0.0f);
		}
		else
		{
			temp[1] = (1.0f);
			temp[0] = (0.0f);
		}
		if (q_values[2] > q_values[3]) // accel or decel
		{
			temp[2] = (1.0f);
			temp[3] = (0.0f);
		}
		else
		{
			temp[2] = (1.0f);
			temp[3] = (0.0f);
		}
	}
	else
		temp[4] = 1.0f;


	pair<vector<float>, bool> p = make_pair(temp, false);

	return p;
}

/// <summary>
/// saves reinforcemnet weights to a file
/// </summary>
void Brain::saveRWeights()
{
	std::ofstream myfile;
	myfile.open("DATA/ReinforcementWeights.csv");
	for (int i = 0; i < RweightsLayer1.size(); i++)
	{
		for (int j = 0; j < RweightsLayer1[i].size(); j++)
		{
			myfile << RweightsLayer1[i][j] << ",";
		}
		myfile<< "\n";
	}
	for (int i = 0; i < RweightsLayer2.size(); i++)
	{
		for (int j = 0; j < RweightsLayer2[i].size(); j++)
		{
			myfile << RweightsLayer2[i][j] << ",";
		}
		myfile << "\n";
	}
	myfile.close();
	cout << "Weights saved" << endl;
}

/// <summary>
/// load backprop weights from file
/// </summary>
void Brain::loadBPWeights()
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
		for (int i = 0; i < numInputs; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				BPweightsLayer1[i][index] = weight;
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < numInputs; i++)
		{
			getline(weightsFile, line);
			//ignore biases
		}
		// layer 2
		for (int i = 0; i < numHidden; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				BPweightsLayer2[i][index] = weight;
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		for (int i = 0; i < numHidden; i++)
		{
			getline(weightsFile, line);
			//ignore bias
		}
	}
	weightsFile.close();
}

/// <summary>
/// load reinforcemnt weights from file
/// </summary>
void Brain::loadRWeights()
{
	vector<string> row;
	string line, word, temp;
	int layer = 0;
	int index = 0;
	float weight = 0.0f;

	weightsFile.open("DATA/ReinforcementWeights.csv");

	while (!weightsFile.eof())
	{
		//layer 1
		for (int i = 0; i < numInputs; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				RweightsLayer1[i][index] = weight;
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		// layer 2
		for (int i = 0; i < numHidden; i++)
		{
			row.clear();
			getline(weightsFile, line);
			//cout << line << std::endl;

			stringstream s(line);

			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				RweightsLayer2[i][index] = (weight);
				//std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
	}
	weightsFile.close();
}

/// <summary>
/// evaluate the neural network based off backprop weights
/// </summary>
/// <param name="t_inputs"> inputs to be processed </param>
/// <returns> calculated outputs </returns>
vector<float> Brain::backprop(vector<float> t_inputs)
{
	float result = 0.0;
	int layer = 0;

	float dot[1] = { 0.0 };
	float soft[1] = { 0.0 };
	float product = 0.0;
	// apply weights input to hidden layer
	for (int i = 0; i < numHidden; i++)
	{
		product = 0.0;
		for (int j = 0; j < numInputs; j++)
		{
			product = t_inputs[j] * BPweightsLayer1[j][i];
			dot[i] += product;
		}
		soft[i] = ReLu(dot[i]);
	}
	SoftMax(soft, numHidden);
	BPOutputs.clear();
	//apply weights hidden to output
	for (int i = 0; i < numOutputs; i++)
	{
		product = 0.0;
		for (int j = 0; j < numHidden; j++)
		{
			product = soft[j] * BPweightsLayer2[j][i];
			result += product;
		}
		result = Sigmoid(result);
		BPOutputs.push_back(result);
		result = 0.0f;
	}
	if ((BPOutputs[4] < BPOutputs[0]) && (BPOutputs[4] < BPOutputs[1]) &&
		(BPOutputs[4] < BPOutputs[2]) && (BPOutputs[4] < BPOutputs[3]))
	{
		if (BPOutputs[0] > BPOutputs[1]) // left or right
		{
			BPOutputs[0] = (1.0f);
			BPOutputs[1] = (0.0f);
		}
		else
		{
			BPOutputs[1] = (1.0f);
			BPOutputs[0] = (0.0f);
		}
		if (BPOutputs[2] > BPOutputs[3]) // accel or decel
		{
			BPOutputs[2] = (1.0f);
			BPOutputs[3] = (0.0f);
		}
		else
		{
			BPOutputs[3] = (1.0f);
			BPOutputs[2] = (0.0f);
		}
	}
	else
		BPOutputs[4] = 1.0f;

	return BPOutputs;
}

/// <summary>
/// the softmax function to be used in neural network evaluation
/// </summary>
/// <param name="data"> data array to be processed </param>
/// <param name="len"> length of array </param>
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

/// <summary>
/// the softmax function to be used in neural network evaluation
/// </summary>
/// <param name="data"> data vector to be processed </param>
/// <param name="len"> length of array </param>
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

/// <summary>
/// the ReLu activation function for the neural networks
/// </summary>
/// <param name="val"> value to be processed </param>
/// <returns> calcu;ated value </returns>
float Brain::ReLu(float val)
{
	if (val < 0) val = 0;
	return val;
}
