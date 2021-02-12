#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "GameState.h"

using namespace std;

class Backprop
{
public:
	Backprop();
	~Backprop();
	void addTrainingData(TrainingData t_new);
	vector<TrainingData> getTrainingData() { return m_trainingData; };
private:
	vector<TrainingData> m_trainingData;
	vector<Data> m_backpropData;

	void loadBPData();
};