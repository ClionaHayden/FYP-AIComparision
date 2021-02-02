#pragma once
#include <vector>
#include "GameState.h"

using namespace std;

class Backprop
{
public:
	Backprop();
	~Backprop();
	void addTrainingData(Data t_new);
private:
	vector<Data> m_trainingData;
};