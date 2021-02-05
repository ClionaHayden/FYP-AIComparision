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
	void addTrainingData(Data t_new);
	vector<Data> getTrainingData() { return m_trainingData; };
private:
	vector<Data> m_trainingData;
	vector<Data> m_backpropData;

	void loadBPData();
};